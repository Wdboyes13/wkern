section .multiboot
align 4
    dd 0x1BADB002          ; magic
    dd 0x0                 ; flags
    dd -(0x1BADB002 + 0x0) ; checksum


section .bss
align 16
kernel_stack_bottom:
    resb 16384
kernel_stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, kernel_stack_top
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang