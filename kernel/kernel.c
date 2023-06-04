#include "../devices/keyboard.h"
#include "../devices/timer.h"
#include "../display/screen.h"
#include "../interrupt/idt.h"
#include "../libc/printf.h"
#include "../libc/string.h"
#include "../memory/gdt.h"

void kernel_main(unsigned long magic, unsigned long addr) {
  terminal_init();
  gdt_init();
  idt_init();
  timer_init(20);
  keyboard_init();
  asm volatile("sti");

  while (1) // CPU idles with a `hlt` loop.
    asm volatile("hlt");
}
