#include "slabs.h"
#include "../libc/debug.h"
#include "../libc/memory.h"

static uint32_t _salloc_internal(slab_node_t **freelist) {
    if (freelist == NULL) {
        warn("salloc: given free-list pointer is NULL");
        return 0;
    }

    slab_node_t *node = *freelist;

    /** No slab is free, time to panic. */
    if (node == NULL) {
        warn("salloc: free-list %p has no free slabs", freelist);
        return 0;
    }

    *freelist = node->next;
    return (uint32_t)node;
}

/**
 *  * Internal generic SLAB deallocator. Assumes the address is valid and
 *   * properly-aligned to the granularity.
 *    */
static void _sfree_internal(slab_node_t **freelist, void *addr) {
    slab_node_t *node = (slab_node_t *)addr;

    /** Simply insert to the head of free-list. */
    node->next = *freelist;
    *freelist = node;
}

/** Page-granularity SLAB free-list. */
static uint32_t page_slab_btm;
static uint32_t page_slab_top;

static slab_node_t *page_slab_freelist;

/** Wrappers for differnet granularities. */
uint32_t salloc_page(void) { return _salloc_internal(&page_slab_freelist); }

/** Wrapper for different granularities. */
void sfree_page(void *addr) {
    if ((uint32_t)addr < page_slab_btm || (uint32_t)addr >= page_slab_top) {
        error("sfree_page: object %p is out of page slab range", addr);
        return;
    }

    if ((uint32_t)addr % PAGE_SIZE != 0) {
        error("sfree_page: object %p is not page-aligned", addr);
        return;
    }

    /** Fill with zero bytes to catch dangling pointers use. */
    memset((char *)addr, 0, PAGE_SIZE);

    _sfree_internal(&page_slab_freelist, addr);
}

/** Initializers for SLAB allocators. */
void page_slab_init(void) {
    page_slab_btm = PAGE_SLAB_MIN;
    page_slab_top = PAGE_SLAB_MAX;

    page_slab_freelist = NULL;

    for (uint32_t addr = page_slab_btm; addr < page_slab_top;
         addr += PAGE_SIZE) {
        sfree_page((char *)addr);
    }
}
