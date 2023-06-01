#include "heap.h"
#include "../drivers/screen.h"

extern uint32_t end_address;
uint32_t placement_address = (uint32_t)&end_address;

uint32_t kmalloc_int(uint32_t size, int align, uint32_t *phys) {
    if (align == 1 &&
        (placement_address &
         0xFFFFF000)) // If the address is not already page-aligned
    {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys) {
        *phys = placement_address;
    }
    uint32_t tmp = placement_address;
    placement_address += size;
    return tmp;
}

uint32_t kmalloc_a(uint32_t size) { return kmalloc_int(size, 1, 0); }

uint32_t kmalloc_p(uint32_t size, uint32_t *phys) {
    return kmalloc_int(size, 0, phys);
}

uint32_t kmalloc_ap(uint32_t size, uint32_t *phys) {
    return kmalloc_int(size, 1, phys);
}

uint32_t kmalloc(uint32_t size) {
    screen_print("palcement is is: ");
    screen_print_decimal(placement_address);
    screen_print("\n");
    screen_print("End is: ");
    screen_print_decimal(end_address);
    screen_print("\n");
    return kmalloc_int(size, 0, 0);
}
