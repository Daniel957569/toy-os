#ifndef MEMORY_H
#define MEMORY_H

#include "../common.h"

void memory_copy(char *source, char *dest, int nbytes);
void memory_set_8(uint8_t *dest, uint8_t val, uint32_t len);
void memory_set_16(uint16_t *dest, uint16_t val, uint32_t len);
void memory_set_32(uint32_t *dest, uint32_t val, uint32_t len);

#endif 
