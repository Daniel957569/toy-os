#include "gdt.h"

static gdt_entry_t gdt[NUM_SEGMENTS];

static gdt_register_t gdtr;

extern void gdt_load(uint32_t gdtr_ptr, uint32_t data_selector_offset,
                     uint32_t code_selector_offset);

/**
 ** Here, BASE and LIMIT are in their complete version, ACCESS represents
 ** the access byte, and FLAGS has the 4-bit granularity flags field in its
 ** higer 4 bits.
 **/
static void gdt_set_entry(int idx, uint32_t base, uint32_t limit,
                          uint8_t access, uint8_t flags) {
  gdt[idx].limit_lo = (uint16_t)(limit & 0xFFFF);
  gdt[idx].base_lo = (uint16_t)(base & 0xFFFF);
  gdt[idx].base_mi = (uint8_t)((base >> 16) & 0xFF);
  gdt[idx].access = (uint8_t)access;
  gdt[idx].limit_hi_flags = (uint8_t)((limit >> 16) & 0x0F);
  gdt[idx].limit_hi_flags |= (uint8_t)(flags & 0xF0);
  gdt[idx].base_hi = (uint8_t)((base >> 24) & 0xFF);
}


void gdt_init() {
  /**
   * Access Byte -
   *   - Pr    = 1: present, must be 1 for valid selectors
   *   - Privl = ?: ring level, 0 for kernel and 3 for user mode
   *   - S     = 1: should be 1 for all non-system segments
   *   - Ex    = ?: executable, 1 for code and 0 for data segment
   *   - DC    =
   *     - Direction bit for data selectors,  = 0: segment spans up
   *     - Conforming bit for code selectors, = 0: can only be executed
   *                                               from ring level set
   *                                               in `Privl` field
   *   - RW    =
   *     - Readable bit for code selectors, = 1: allow reading
   *     - Writable bit for data selectors, = 1: allow writing
   *   - Ac    = 0: access bit, CPU sets it to 1 when accessing it
   *   Hence, the four values used below.
   *
   * Flags -
   *   - Gr = 1: using page granularity
   *   - Sz = 1: in 32-bit protected mode
   *   Hence, 0b1100 -> 0xC for all these four segments.
   */
  gdt_set_entry(SEGMENT_UNUSED, 0u, 0u, 0u, 0u); /** 0-th entry unused. */
  gdt_set_entry(SEGMENT_KCODE, 0u, 0xFFFFF, 0x9A, 0xC0);
  gdt_set_entry(SEGMENT_KDATA, 0u, 0xFFFFF, 0x92, 0xC0);
  gdt_set_entry(SEGMENT_UCODE, 0u, 0xFFFFF, 0xFA, 0xC0);
  gdt_set_entry(SEGMENT_UDATA, 0u, 0xFFFFF, 0xF2, 0xC0);

  /** Setup the GDTR register value. */
  gdtr.boundary = (sizeof(gdt_entry_t) * NUM_SEGMENTS) - 1;
  gdtr.base = (uint32_t)&gdt;

  gdt_load((uint32_t)&gdtr, SEGMENT_KDATA << 3, SEGMENT_KCODE << 3);
}
