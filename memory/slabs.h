#ifndef SLABS_H
#define SLABS_H

#include "../libc/common.h"
#include "paging.h"

/** Reserve kheap top 4MiB for the page slabs. */
#define PAGE_SLAB_MAX KMEM_MAX
#define PAGE_SLAB_MIN (KMEM_MAX - 0x00400000)

struct slab_node {
    struct slab_node *next;
};
typedef struct slab_node slab_node_t;

void page_slab_init();

uint32_t salloc_page();
void sfree_page(void *addr);

#endif
