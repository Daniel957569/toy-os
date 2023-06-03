
.global gdt_load
.type gdt_load, @function
gdt_load:
    
    /** Get first two arguments. */
    movl 4(%esp), %edx
    movl 8(%esp), %eax

    /** Load the GDT. */
    lgdt (%edx)

    /** Load all data segment selectors. */
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    /** Load stack segment as well. */
    movw %ax, %ss

    
    /** Load code segment by doing a far jump. */
    pushl 12(%esp)  /** Push FAR pointer on stack. */
    pushl $.setcs   /** Push offset of FAR JMP. */
    ljmp *(%esp)    /** Perform FAR JMP. */

.setcs:

    addl $8, %esp   /** Restore stack. */
    
    ret
