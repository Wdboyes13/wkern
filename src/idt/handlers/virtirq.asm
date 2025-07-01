global virtirq
extern virtnet_irq_handler
VirtnetHandler:
    cli                 ; Disable interrupts
    pusha               ; Push all general-purpose registers
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10        ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call virtnet_irq_handler
    
    pop gs
    pop fs
    pop es
    pop ds
    popa

    mov al, 0x20        ; Send EOI
    out 0x20, al

    sti                 ; Re-enable interrupts
    iretd