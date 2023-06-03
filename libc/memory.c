#include "memory.h"

void *memset(void *dst, unsigned char c, size_t count) {
  unsigned char *dst_copy = (unsigned char *)dst;

  while (count-- > 0)
    *dst_copy++ = c;

  return dst;
}

void *memcpy(void *dst, const void *src, size_t count) {
  unsigned char *dst_copy = (unsigned char *)dst;
  unsigned char *src_copy = (unsigned char *)src;

  while (count-- > 0)
    *dst_copy++ = *src_copy++;

  return dst;
}

void *memmove(void *dst, const void *src, size_t count) {
  unsigned long int dstp = (long int)dst;
  unsigned long int srcp = (long int)src;

  if (dstp - srcp >= count) 
    dst = memcpy(dst, src, count);
  else { 
    unsigned char *dst_copy = ((unsigned char *)dst) + count;
    unsigned char *src_copy = ((unsigned char *)src) + count;
    while (count-- > 0)
      *dst_copy-- = *src_copy--;
  }

  return dst;
}

int memcmp(const void *ptr1, const void *ptr2, size_t count) {
  const char *ptr1_cast = (const char *)ptr1;
  const char *ptr2_cast = (const char *)ptr2;

  char b1 = 0, b2 = 0;
  while (count-- > 0) {
    b1 = *ptr1_cast++;
    b2 = *ptr2_cast++;
    if (b1 != b2)
      return ((int)b1) - ((int)b2);
  }

  return ((int)b1) - ((int)b2);
}
