global irq14_handler
extern irq14_handler_C
irq14_handler:
    cli
    pusha
    call irq14_handler_C
    popa
    sti
    iret