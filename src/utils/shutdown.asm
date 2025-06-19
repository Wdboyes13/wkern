[BITS 16]
section .trampoline
global shutdown
shutdown:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov sp, 0x7C00

    mov ax, 0x5301
    xor bx, bx
    int 0x15

    mov ax, 0x530E
    xor bx, bx
    int 0x15

    mov ax, 0x5307
    mov bx, 0x0001
    mov cx, 0x0003
    int 0x15

    mov ax, 0x1000
    int 0x15

.hang:
    cli
    hlt
    jmp .hang
