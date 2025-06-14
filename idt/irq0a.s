.global irq0_handler
irq0_handler:
    pusha               # save all general registers
    push %ds
    push %es

    movw $0x10, %ax     # load data segment selector (kernel data)
    movw %ax, %ds
    movw %ax, %es

    call irq0_handler_c # call C handler

    pop %es
    pop %ds
    popa

    # EOI sent in C handler already, no need here

    iret                # return from interrupt
    