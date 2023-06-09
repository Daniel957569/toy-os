#include "idt.h"
#include "../libc/memory.h"
#include "../libc/port.h"
#include "../memory/gdt.h"

static idt_gate_t idt[NUM_GATE_ENTRIES];

static idt_register_t idtr;

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

/** Extern our PIC IRQ handlers written in ASM `irq-stub.s`. */
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

static void idt_set_gate(int idx, uint32_t base, uint16_t selector,
                         uint8_t flags) {
  idt[idx].base_lo = (uint16_t)(base & 0xFFFF);
  idt[idx].selector = (uint16_t)selector;
  idt[idx].zero = (uint8_t)0;
  idt[idx].flags = (uint8_t)flags;
  idt[idx].base_hi = (uint16_t)((base >> 16) & 0xFFFF);
}

extern void idt_load(uint32_t idtr_ptr);

void idt_init() {
  outb(0x20, 0x11); /** Initialize master PIC in cascade mode. */
  outb(0xA0, 0x11); /** Initialize slave  PIC in cascade mode. */
  outb(0x21, 0x20); /** Master PIC mapping offset = 0x20. */
  outb(0xA1, 0x28); /** Slave  PIC mapping offset = 0x28. */
  outb(0x21, 0x04); /** Tell master PIC that slave PIC at IRQ # 2. */
  outb(0xA1, 0x02); /** Tell slave PIC its cascade identity at 2. */
  outb(0x21, 0x01); /** Set master PIC in 8086/88 mode. */
  outb(0xA1, 0x01); /** Set slave  PIC in 8086/88 mode. */

  outb(0x21, 0x0); /** Set masking of master PIC. */
  outb(0xA1, 0x0); /** Set masking of slave  PIC. */

  memset(idt, 0, sizeof(idt_gate_t) * 256);

  idt_set_gate(0, (uint32_t)isr0, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(1, (uint32_t)isr1, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(2, (uint32_t)isr2, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(3, (uint32_t)isr3, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(4, (uint32_t)isr4, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(5, (uint32_t)isr5, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(6, (uint32_t)isr6, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(7, (uint32_t)isr7, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(8, (uint32_t)isr8, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(9, (uint32_t)isr9, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(10, (uint32_t)isr10, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(11, (uint32_t)isr11, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(12, (uint32_t)isr12, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(13, (uint32_t)isr13, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(14, (uint32_t)isr14, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(15, (uint32_t)isr15, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(16, (uint32_t)isr16, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(17, (uint32_t)isr17, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(18, (uint32_t)isr18, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(19, (uint32_t)isr19, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(20, (uint32_t)isr20, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(21, (uint32_t)isr21, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(22, (uint32_t)isr22, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(23, (uint32_t)isr23, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(24, (uint32_t)isr24, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(25, (uint32_t)isr25, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(26, (uint32_t)isr26, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(27, (uint32_t)isr27, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(28, (uint32_t)isr28, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(29, (uint32_t)isr29, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(30, (uint32_t)isr30, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(31, (uint32_t)isr31, SEGMENT_KCODE << 3, 0x8E);

  /** These are for PIC IRQs (remapped). */
  idt_set_gate(32, (uint32_t)irq0, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(33, (uint32_t)irq1, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(34, (uint32_t)irq2, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(35, (uint32_t)irq3, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(36, (uint32_t)irq4, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(37, (uint32_t)irq5, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(38, (uint32_t)irq6, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(39, (uint32_t)irq7, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(40, (uint32_t)irq8, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(41, (uint32_t)irq9, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(42, (uint32_t)irq10, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(43, (uint32_t)irq11, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(44, (uint32_t)irq12, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(45, (uint32_t)irq13, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(46, (uint32_t)irq14, SEGMENT_KCODE << 3, 0x8E);
  idt_set_gate(47, (uint32_t)irq15, SEGMENT_KCODE << 3, 0x8E);

  idtr.boundary = (sizeof(idt_gate_t) * NUM_GATE_ENTRIES) - 1;
  idtr.base = (uint32_t)&idt;

  idt_load((uint32_t)&idtr);
}
