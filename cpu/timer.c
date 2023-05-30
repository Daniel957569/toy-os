#include "timer.h"
#include "../drivers/ports.h"
#include "../drivers/screen.h"
#include "../kernel/util.h"
#include "isr.h"

uint32_t tick = 0;

static void timer_callback(registers_t regs) {
    tick++;
    screen_print("Tick: ");

    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
    screen_print(tick_ascii);
    screen_print("\n");
}

void init_timer(uint32_t freq) {
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    /* Send the command */
    port_byte_out(0x43, 0x36); /* Command port */
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
