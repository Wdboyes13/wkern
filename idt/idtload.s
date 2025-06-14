    .globl idt_load
    .extern idt_ptr
    .text
idt_load:
    lidt idt_ptr
    ret