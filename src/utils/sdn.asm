[BITS 32]
global _sdn_

_sdn_:
    cli                         ; Disable interrupts
    mov eax, cr0
    and eax, 0xFFFFFFFE         ; Clear PE bit (disable protected mode)
    mov cr0, eax

    ; Far jump to real mode shutdown code
    jmp 0x0000:0x7C00  