#include "timer.h"
#include "../interrupt/idt.h"
#include "../interrupt/isr.h"
#include "../libc/port.h"
#include "../libc/printf.h"

int tick = 0;

static void timer_interrupt_handler(interrupt_state_t *state) {
  (void)state; /** Unused. */
  printf("\ntick: %d", tick);
  tick++;
}

void timer_init(void) {
  isr_register(INT_NO_TIMER, &timer_interrupt_handler);

  uint16_t divisor = 1193182 / TIMER_FREQ_HZ;

  outb(0x43, 0x36); /** Run in mode 3. */

  /** Sends frequency divisor, in lo | hi order. */
  outb(0x40, (uint8_t)(divisor & 0xFF));
  outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}
