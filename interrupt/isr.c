#include "isr.h"
#include "../libc/debug.h"
#include "idt.h"
#include "../libc/port.h"

isr_t isr_table[NUM_GATE_ENTRIES] = {NULL};

void isr_register(uint8_t int_no, isr_t handler) {
  if (isr_table[int_no] != NULL) {
    error("isr: handler for interrupt # %#x already registered", int_no);
    return;
  }
  isr_table[int_no] = handler;
}

static void _pic_send_eoi(uint8_t irq_no) {
  if (irq_no >= 8)
    outb(0xA0, 0x20); 
  outb(0x20, 0x20);
}

void isr_handler(interrupt_state_t *state) {
  uint8_t int_no = state->int_no;

  /** An exception interrupt. */
  if (int_no <= 31) {

    /** Panic if no actual ISR is registered. */
    if (isr_table[int_no] == NULL) 
      error("missing handler for ISR interrupt # %#x", int_no);
    else
      isr_table[int_no](state);

    /** An IRQ-translated interrupt from external device. */
  } else if (int_no <= 47) {
    uint8_t irq_no = state->int_no - 32;

    /** Call actual ISR if registered. */
    if (isr_table[int_no] == NULL) 
      error("missing handler for IRQ interrupt # %#x", int_no);
    else
      isr_table[int_no](state);

    _pic_send_eoi(irq_no); /** Send back EOI signal to PIC. */
  }
}
