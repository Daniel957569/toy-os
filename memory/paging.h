#ifndef PAGING_H
#define PAGING_H

#include "../libc/common.h"

#define PAGE_SIZE 4096

#define PTES_PER_PAGE 1024
#define PDES_PER_PAGE 1024

#define PHYS_MAX 0x08000000 /** 128MiB physical memory. */
#define NUM_FRAMES (PHYS_MAX / PAGE_SIZE)

#define KMEM_MAX 0x00800000 /** 8MiB reserved for the kernel. */

#define ADDR_PAGE_OFFSET(addr) ((addr)&0x00000FFF)
#define ADDR_PAGE_NUMBER(addr) ((addr) >> 12)

#define ADDR_PDE_INDEX(addr) (ADDR_PAGE_NUMBER(addr) / 1024)
#define ADDR_PTE_INDEX(addr) (ADDR_PAGE_NUMBER(addr) % 1024)

#define ADDR_PAGE_ALIGNED(addr) (ADDR_PAGE_OFFSET(addr) == 0)

#define ADDR_PAGE_ROUND_DN(addr) ((addr)&0xFFFFF000)
#define ADDR_PAGE_ROUND_UP(addr) (ADDR_PAGE_ROUND_DN((addr) + 0x00000FFF))

#define ENTRY_FRAME_ADDR(entry) ((uint32_t)(entry).frame << 12)

struct page_table_entry {
  uint32_t present : 1;  /** Set -> present in memory. */
  uint32_t writable : 1; /** Set -> user writable. (read/write bit) */
  uint32_t user : 1;     /** Set -> user accessible. */
  uint32_t unused0 : 2;  /** Unused 2 caching bits. */
  uint32_t accessed : 1; /** Set -> accessed sinced mapped. */
  uint32_t dirty : 1;    /** Set -> page has been written to. */
  uint32_t unused1 : 5;  /** Unused 5 misc bits. */
  uint32_t frame : 20;   /** Physical frame number of the page. */
} __attribute__((packed));
typedef struct page_table_entry pte_t;

struct page_directory_entry {
  uint32_t present : 1;  /** Set -> present in memory. */
  uint32_t writable : 1; /** Set -> user writable. (read/write bit) */
  uint32_t user : 1;     /** Set -> user accessible. */
  uint32_t unused0 : 2;  /** Unused 2 caching bits. */
  uint32_t accessed : 1; /** Set -> accessed sinced mapped. */
  uint32_t unused1 : 1;  /** Unused bit. */
  uint32_t size : 1;     /** 0 -> using 4KiB page size. */
  uint32_t unused2 : 4;  /** Unused 4 misc bits. */
  uint32_t frame : 20;   /** Physical frame number of level-2 table. */
} __attribute__((packed));
typedef struct page_directory_entry pde_t;

extern uint32_t kheap_curr;
extern pde_t *kernel_pgdir;

void paging_init();

pte_t *paging_walk_pgdir_at_boot(pde_t *pgdir, uint32_t vaddr, bool alloc);

void paging_switch_pgdir(pde_t *pgdir);

#endif
