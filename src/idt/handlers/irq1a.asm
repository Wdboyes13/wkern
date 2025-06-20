global irq1_handler
extern irq1_handler_c
irq1_handler:
    pusha
    call irq1_handler_c
    mov al, 0x20
    out 0x20, al
    popa
    iretd
    