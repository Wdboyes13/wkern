
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

global irq0_handler
extern irq0_handler_c
section .text
irq0_handler:
    pusha                   ; Save general-purpose registers
    push ds
    push es

    mov ax, 0x10            ; Kernel data segment selector
    mov ds, ax
    mov es, ax

    call irq0_handler_c     ; Call C handler (must be extern in C)

    pop es
    pop ds
    popa

    iret                    ; Return from interrupt
    