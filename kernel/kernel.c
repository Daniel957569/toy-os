#include "../boot/multiboot.h"
#include "../devices/keyboard.h"
#include "../devices/timer.h"
#include "../display/screen.h"
#include "../interrupt/idt.h"
#include "../libc/debug.h"
#include "../libc/port.h"
#include "../libc/printf.h"
#include "../libc/string.h"
#include "../memory/gdt.h"
#include "../memory/kheap.h"
#include "../memory/paging.h"

static inline void _init_message(char *msg) {
    printf("[");
    cprintf(VGA_COLOR_BLUE, "INIT");
    printf("] %s...", msg);
}

static inline void _init_message_ok(void) { cprintf(VGA_COLOR_GREEN, " OK\n"); }

void kernel_main(unsigned long magic, unsigned long addr) {
    terminal_init();

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        error("invalid bootloader magic: %#x", magic);
        return;
    }

    multiboot_info_t *mbi = (multiboot_info_t *)addr;

    debug_init(mbi);

    gdt_init();
    idt_init();

    paging_init();
    kheap_init();

    printf("\nKallocing arr1 - 128 bytes...\n");
    char *arr1 = (char *)kalloc(128 * sizeof(char));
    strncpy(arr1, "hello\n", 127);

    kfree(arr1);

    while (1) // CPU idles with a `hlt` loop.
        asm volatile("hlt");
}
