extern kputchar2
extern idt_ptrn
section .text
    global _load_interrupts

_load_interrupts:
    push byte 'A'
    call kputchar2
    add esp, 4

    cli

    push byte 'A'
    call kputchar2
    add esp, 4


    push byte 'B'
    call kputchar2
    add esp, 4

    lidt [idt_ptrn]

    push byte 'B'
    call kputchar2
    add esp, 4

    push byte 'C'
    call kputchar2
    add esp, 4

    mov al, 0xFE
    out 0x21, al

    mov al, 0xFF
    out 0xA1, al

    push byte 'C'
    call kputchar2
    add esp, 4

    push byte 'D'
    call kputchar2
    add esp, 4

    sti

    push byte 'D'
    call kputchar2
    add esp, 4

    push byte 'E'
    call kputchar2
    add esp, 4

    int 0x20

    push byte 'E'
    call kputchar2
    add esp, 4


    ret
