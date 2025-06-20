global irq0_handler
extern irq0_handler_c
section .text
irq0_handler:
    pusha                   ; Save general-purpose registers
    push ds
    push es

    mov ax, 0x10            ; Kernel data segment selector
    mov ds, ax
    mov es, ax

    call irq0_handler_c     ; Call C handler (must be extern in C)

    pop es
    pop ds
    popa

    iret                    ; Return from interrupt
    