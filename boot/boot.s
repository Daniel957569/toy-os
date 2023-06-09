/**
 * Declare the multiboot header. It should contain "magic", "flags", and
 * "checksum" in order. GRUB will search for this header in the first 8KiB of
 * the kernel file, aligned to 32-bit boundary.
 */
.set MAGIC,     0x1BADB002

.set ALIGN,     1<<0    /** Align modules on page boundaries. */
.set MEMINFO,   1<<1    /** Provide memory map. */
.set FLAGS,     ALIGN | MEMINFO

.set CHECKSUM,  -(MAGIC + FLAGS)    /** Add up to 0. */

/** Header section here. */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM


/**
 * The kernel must provide a temporary stack for code execution at booting,
 * when the entire virtual memory system and user mode execution has not been
 * set up yet. We make a 16KiB stack space here.
 * 
 * It will grow downwards from current 'stack_hi'. The stack must be
 * aligned to 16 Bytes according to the System V ABI standard.
 */
.section .bss
.align 16
stack_lo:
.skip 16384     /** 16 KiB. */
stack_hi:


/**
 * Our linker script will use '_start' as the entry point to the kernel. No
 * need to return from the _start function because the bootloader has gone
 * at this time point.
 *
 * We will be in 32-bit x86 protected mode. Interrupts disabled, paging
 * disabled, and processor state as defined in multiboot specification 3.2.
 * The kernel has full control over the machine.
 */
.section .text
.global _start
.type _start, @function
_start:

    /** Setup the kernel stack by setting ESP to our 'stack_hi' symbol. */
    movl $stack_hi, %esp

    xor %ebp, %ebp

    pushl %ebx
    pushl %eax

    /**
     * Other processor state modifications and runtime supports (such as
     * enabling paging) should go here. Make sure your ESP is still 16 Bytes
     * aligned.
     */
    
    /** Jump to the 'kernel_main' function. */
    call kernel_main

    /**
     * Put the computer into infinite loop if it has nothing more to do after
     * the main function. The following is such a trick.
     */
    cli         /** Disable interrupts. */
halt:
    hlt         /** Halt and wait for the next interrupt. */
    jmp halt    /** Jump to 'halt' if any non-maskable interrupt occurs. */


/**
 * Set the size of the '_start' symbol as the current location '.' minus
 * the starting point. Useful when later debugging or implementing call
 * stack tracing.
 */
.size _start, . - _start
