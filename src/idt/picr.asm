
; WKern - A Bare Metal OS / Kernel I am making (For Fun)
;Copyright (C) 2025  Wdboyes13

; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; any later version.

; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.

; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
    