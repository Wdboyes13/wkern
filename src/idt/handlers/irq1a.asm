
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

global Irq1Handler
extern Irq1HandlerC

Irq1Handler:
    cli                 ; Disable interrupts
    pusha               ; Push all general-purpose registers
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10        ; Kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call Irq1HandlerC

    pop gs
    pop fs
    pop es
    pop ds
    popa

    mov al, 0x20        ; Send EOI
    out 0x20, al

    sti                 ; Re-enable interrupts
    iretd
