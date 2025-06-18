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

    ; Unmask all
    mov al, 0x00
    mov dx, 0x21
    out dx, al
    mov dx, 0xA1
    out dx, al

    ret