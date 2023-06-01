#include "memory.h"

void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set_8(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;
    for (; len != 0; len--)
        *temp++ = val;
}

void memory_set_16(uint16_t *dest, uint16_t val, uint32_t len) {
    uint16_t *temp = (uint16_t *)dest;
    for (; len != 0; len--)
        *temp++ = val;
}


void memory_set_32(uint32_t *dest, uint32_t val, uint32_t len) {
    uint32_t *temp = (uint32_t *)dest;
    for (; len != 0; len--)
        *temp++ = val;
}
