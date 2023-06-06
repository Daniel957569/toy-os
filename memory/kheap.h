#ifndef KHEAP_H
#define KHEAP_H

#include "../libc/common.h"

#define KHEAP_MAGIC 0xFBCA0739

struct free_list_node_header {
    size_t size;
    bool free;
    struct free_list_node_header *next;
    uint32_t magic;
};
typedef struct free_list_node_header fl_header_t;

/** Pointer arithmetics helper macros. */
#define HEADER_TO_OBJECT(header) ((header) + sizeof(fl_header_t))
#define OBJECT_TO_HEADER(object) ((object) - sizeof(fl_header_t))

void kheap_init();

uint32_t kalloc(size_t size);
void kfree(void *addr);

#endif
