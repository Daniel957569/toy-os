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

  _init_message("setting up virtual memory using paging");
  paging_init();
  _init_message_ok();
  info("supporting physical memory size: %3dMiB", NUM_FRAMES * 4 / 1024);
  info("reserving memory for the kernel: %3dMiB", KMEM_MAX / 1024 / 1024);

  asm volatile("sti");

  uint32_t faulty_addr = 0x0CDE0123;

  printf("\nKernel reading an unmapped address!\n");
  int dummy = *((int *)faulty_addr);
  printf("%d\n", dummy);

  while (1) 
    asm volatile("hlt");
}
