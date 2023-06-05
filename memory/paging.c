#include "paging.h"
#include "../interrupt/isr.h"
#include "../libc/debug.h"
#include "../libc/memory.h"

uint32_t kheap_curr;
static uint8_t *frame_bitmap;
pde_t *kernel_pgdir; /** Allocated at paging init. */

#define BITMAP_OUTER_IDX(frame_num) ((frame_num) / 8)
#define BITMAP_INNER_IDX(frame_num) ((frame_num) % 8)

/** Set a frame as used. */
static inline void frame_bitmap_set(uint32_t frame_num) {
    size_t outer_idx = BITMAP_OUTER_IDX(frame_num);
    size_t inner_idx = BITMAP_INNER_IDX(frame_num);
    frame_bitmap[outer_idx] |= (1 << (7 - inner_idx));
}

/** Clear a frame as free. */
static inline void frame_bitmap_clear(uint32_t frame_num) {
    size_t outer_idx = BITMAP_OUTER_IDX(frame_num);
    size_t inner_idx = BITMAP_INNER_IDX(frame_num);
    frame_bitmap[outer_idx] &= ~(1 << (7 - inner_idx));
}

/** Returns true if a frame is in use, otherwise false. */
static inline bool frame_bitmap_check(uint32_t frame_num) {
    size_t outer_idx = BITMAP_OUTER_IDX(frame_num);
    size_t inner_idx = BITMAP_INNER_IDX(frame_num);
    return frame_bitmap[outer_idx] & (1 << (7 - inner_idx));
}

static uint32_t frame_bitmap_alloc(void) {
    for (size_t i = 0; i < (NUM_FRAMES / 8); ++i) {
        if (frame_bitmap[i] == 0xFF)
            continue;
        for (size_t j = 0; j < 8; ++j) {
            if ((frame_bitmap[i] & (1 << (7 - j))) == 0) {
                /** Found a free frame. */
                uint32_t frame_num = i * 8 + j;
                frame_bitmap_set(frame_num);
                return frame_num;
            }
        }
    }

    return NUM_FRAMES;
}

static uint32_t _kalloc_temp(size_t size, bool page_align) {
    /** If `page_align` is set, return an aligned address. */
    if (page_align && !ADDR_PAGE_ALIGNED(kheap_curr))
        kheap_curr = ADDR_PAGE_ROUND_UP(kheap_curr);

    /** If exceeds the 8MiB kernel memory boundary, panic. */
    if (kheap_curr + size > KMEM_MAX)
        error("_kalloc_temp: kernel memory exceeds boundary");

    uint32_t temp = kheap_curr;
    kheap_curr += size;
    return temp;
}

pte_t *paging_walk_pgdir_at_boot(pde_t *pgdir, uint32_t vaddr, bool alloc) {
    size_t pde_idx = ADDR_PDE_INDEX(vaddr);
    size_t pte_idx = ADDR_PTE_INDEX(vaddr);

    /** If already has the level-2 table, return the correct PTE. */
    if (pgdir[pde_idx].present != 0) {
        pte_t *pgtab = (pte_t *)ENTRY_FRAME_ADDR(pgdir[pde_idx]);
        return &pgtab[pte_idx];
    }

    if (!alloc)
        return NULL;

    pte_t *pgtab = (pte_t *)_kalloc_temp(sizeof(pte_t) * PTES_PER_PAGE, true);
    assert(pgtab != NULL);
    memset(pgtab, 0, sizeof(pte_t) * PTES_PER_PAGE);

    pgdir[pde_idx].present = 1;
    pgdir[pde_idx].writable = 0;
    pgdir[pde_idx].user = 1; /** Just allow user access on all PDEs. */
    pgdir[pde_idx].frame = ADDR_PAGE_NUMBER((uint32_t)pgtab);

    return &pgtab[pte_idx];
}

pde_t *kernel_pgdir; /** Allocated at paging init. */

/** Switch the current page directory to the given one. */
inline void paging_switch_pgdir(pde_t *pgdir) {
    assert(pgdir != NULL);
    asm volatile("movl %0, %%cr3" : : "r"(pgdir));
}

static void page_fault_handler(interrupt_state_t *state) {
    /** The CR2 register holds the faulty address. */
    uint32_t faulty_addr;
    asm("movl %%cr2, %0" : "=r"(faulty_addr) :);

    /**
     - bit 0: page present -> 1, otherwise 0
     - bit 1: is a write operation -> 1, read -> 0
     - bit 2: is from user mode -> 1, kernel -> 0
     */
    bool present = state->err_code & 0x1;
    bool write = state->err_code & 0x2;
    bool user = state->err_code & 0x4;

    info("Caught page fault {\n"
         "  faulty addr = %p\n"
         "  present: %d\n"
         "  write:   %d\n"
         "  user:    %d\n"
         "}",
         faulty_addr, present, write, user);

    panic("page fault not handled!");
}

void paging_init(void) {
    kheap_curr = ADDR_PAGE_ROUND_UP((uint32_t)elf_sections_end);
    printf("\n%d", kheap_curr);

    frame_bitmap = (uint8_t *)_kalloc_temp(NUM_FRAMES / 8, false);
    memset(frame_bitmap, 0, NUM_FRAMES / 8);

    kernel_pgdir = (pde_t *)_kalloc_temp(sizeof(pde_t) * PDES_PER_PAGE, true);
    memset(kernel_pgdir, 0, sizeof(pde_t) * PDES_PER_PAGE);

    /** alloc kernel memory. */
    uint32_t addr = 0;
    while (addr < KMEM_MAX) {
        uint32_t frame_num = frame_bitmap_alloc();
        assert(frame_num < NUM_FRAMES);
        pte_t *pte = paging_walk_pgdir_at_boot(kernel_pgdir, addr, true);
        assert(pte != NULL);

        /** Update the bits in this PTE. */
        pte->present = 1;
        pte->writable = 0; /** Has no affect. */
        pte->user = 0;
        pte->frame = frame_num;

        addr += PAGE_SIZE;
    }

    /**
     alloc phys memory.
     */
    while (addr < PHYS_MAX) {
        pte_t *pte = paging_walk_pgdir_at_boot(kernel_pgdir, addr, true);
        assert(pte != NULL);

        /** Update the bits in this PTE. */
        pte->present = 1;
        pte->writable = 0; /** Has no affect. */
        pte->user = 0;
        pte->frame = ADDR_PAGE_NUMBER(addr);

        addr += PAGE_SIZE;
    }

    isr_register(INT_NO_PAGE_FAULT, &page_fault_handler);
    paging_switch_pgdir(kernel_pgdir);

    /** enable paging. */
    uint32_t cr0;
    asm volatile("movl %%cr0, %0" : "=r"(cr0) :);
    cr0 |= 0x80000001;
    asm volatile("movl %0, %%cr0" : : "r"(cr0));
}
