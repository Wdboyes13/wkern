section .text
    global _picr

_picr:
    ; Start init
    mov al, 0x11
    mov dx, 0x20
    out dx, al
    mov dx, 0xA0
    out dx, al

    ; Set vector offset
    mov al, 0x20
    mov dx, 0x21
    out dx, al
    mov al, 0x28
    mov dx, 0xA1
    out dx, al

    ; Tell master/slave PICs about each other
    mov al, 0x04
    mov dx, 0x21
    out dx, al
    mov al, 0x02
    mov dx, 0xA1
    out dx, al

    ; Set 8086 mode
    mov al, 0x01
    mov dx, 0x21
    out dx, al
    mov dx, 0xA1
    out dx, al

    ; Unmask only IRQ0 and IRQ1 (mask 0xFC = 11111100b)
    mov al, 0xFC
    out 0x21, al      ; Master PIC

    mov al, 0xFF
    out 0xA1, al      ; Slave PIC (mask all)

    ret
    