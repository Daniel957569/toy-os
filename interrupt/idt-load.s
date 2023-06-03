.global idt_load
.type idt_load, @function
idt_load:
    
    /** Get the argument. */
    movl 4(%esp), %eax

    /** Load the IDT. */
    lidt (%eax)

    ret
