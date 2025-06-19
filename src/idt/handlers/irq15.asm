global irq15_handler
extern irq15_handler_C
irq15_handler:
    cli
    pusha
    call irq15_handler_C
    popa
    sti
    iret