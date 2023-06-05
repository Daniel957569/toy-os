#include "../devices/keyboard.h"
#include "../devices/timer.h"
#include "../display/screen.h"
#include "../interrupt/idt.h"
#include "../libc/port.h"
#include "../libc/printf.h"
#include "../libc/string.h"
#include "../memory/gdt.h"
#include "../boot/multiboot.h"
#include "../libc/debug.h"

void kernel_main(unsigned long magic, unsigned long addr) {
  /** Initialize VGA text-mode terminal support. */
  terminal_init();

  /** Double check the multiboot magic number. */
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
    error("invalid bootloader magic: %#x", magic);
    return;
  }

  /** Get pointer to multiboot info. */
  multiboot_info_t *mbi = (multiboot_info_t *)addr;

  /** Initialize debugging utilities. */
  debug_init(mbi);

  assert(0);

  printf("This line should not be displayed!");
}
