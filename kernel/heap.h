#ifndef HEAP_H
#define HEAP_H

#include "../common.h"

uint32_t kmalloc_int(uint32_t size, int align, uint32_t *phys);

/**
   Allocate a chunk of memory, size in size. The chunk must be
   page aligned.
**/
uint32_t kmalloc_a(uint32_t size);

/**
   Allocate a chunk of memory, size in size. The physical address
   is returned in phys. Phys MUST be a valid pointer to uint32_t!
**/
uint32_t kmalloc_p(uint32_t size, uint32_t *phys);

/**
   Allocate a chunk of memory, size in size. The physical address
   is returned in phys. It must be page-aligned.
**/
uint32_t kmalloc_ap(uint32_t size, uint32_t *phys);

/**
   General allocation function.
**/
uint32_t kmalloc(uint32_t size);

#endif
