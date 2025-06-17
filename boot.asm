section .multiboot
align 4
    dd 0x1BADB002          ; magic
    dd 0x0                 ; flags
    dd -(0x1BADB002 + 0x0) ; checksum

section .text
global _start
extern kernel_main

_start:
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang