#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"

void *memset(void *dst, unsigned char c, size_t count);
void *memcpy(void *dst, const void *src, size_t count);
void *memmove(void *dst, const void *src, size_t count);
int memcmp(const void *ptr1, const void *ptr2, size_t count);


#endif
