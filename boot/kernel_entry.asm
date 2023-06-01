global _start
global end_address
[bits 32]
_start:
    [extern main]
    call main ; Calls the C function. The linker will know where it is placed in memory
    jmp $

end_address:
