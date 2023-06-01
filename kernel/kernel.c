#include "../cpu/idt.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "paging.h"

int main() {
    clear_screen();
    isr_install();
    irq_install();
    screen_print("Hello, paging world!\n");
    // initialise_paging();

    // uint32_t *ptr = (uint32_t *)0xA0000000;
    // uint32_t do_page_fault = *ptr;

    return 0;
}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        screen_print("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
    screen_print("You said: ");
    screen_print(input);
    screen_print("\n> ");
}
