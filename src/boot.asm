
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

section .multiboot
align 4
    dd 0x1BADB002          ; magic
    dd 0x0                 ; flags
    dd -(0x1BADB002 + 0x0) ; checksum


section .bss
align 16
kernel_stack_bottom:
    resb 16384
kernel_stack_top:

section .text
global _start
extern kernel_main

_start:
    mov esp, kernel_stack_top
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang