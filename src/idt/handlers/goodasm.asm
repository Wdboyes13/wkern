global isr3_stub
extern isr3_handler

section .text
isr3_stub:
    pusha              ; push all general-purpose registers
    call isr3_handler  ; call the C handler
    popa               ; restore registers
    iret               ; return from interrupt