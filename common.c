#include "common.h"
#include "./drivers/screen.h"

extern void panic(char *message, char *file, uint32_t line) {
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    screen_print("PANIC(");
    screen_print(message);
    screen_print(") at ");
    screen_print(file);
    screen_print(":");
    screen_print_decimal(line);
    screen_print("\n");
    // Halt by going into an infinite loop.
    for (;;)
        ;
}

extern void panic_assert(char *file, uint32_t line, char *desc) {
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    screen_print("ASSERTION-FAILED(");
    screen_print(desc);
    screen_print(") at ");
    screen_print(file);
    screen_print(":");
    screen_print_decimal(line);
    screen_print("\n");
    // Halt by going into an infinite loop.
    for (;;)
        ;
}
