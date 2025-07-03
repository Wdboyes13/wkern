/*
WKern - A Bare Metal OS / Kernel I am maKing (For Fun)
Copyright (C) 2025  Wdboyes13

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <io/kio.h>
/**
 * @brief Kernel Panics with Message
 * @param Message - Panic Error Message
 */
__attribute__((noreturn)) void Panic(const char *msg) {
    Kprintf("\n\n** KERNEL PANIC **\n");
    Kprintf("Message: ");
    Kprintf(msg);
    Kputchar('\n');
    Kprintf("System Halted");

    for (;;) {
        __asm__ volatile("cli\n"
                         "hlt");
    };
}