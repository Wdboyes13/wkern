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
#include <utils/util.h>
/// @brief  Prints an ASCII Happy Face
void draw_smile() {
    Kputchar(0xDB);
    Kprintf("        ");
    Kputchar(0xDB);
    Kputchar('\n');
    Kputchar('\n');
    Kputchar(0xDB);
    Kprintf("        ");
    Kputchar(0xDB);
    Kputchar('\n');
    Kputchar(' ');
    Kputchar(0xDB);
    Kputchar(0xDB);
    Kputchar(0xDB);
    Kputchar(0xDB);
    Kputchar(0xDB);
    Kputchar(0xDB);
    Kputchar(0xDB);
    Kputchar(0xDB);
    Kputchar(' ');
    Kputchar('\n');
}
