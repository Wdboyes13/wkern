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

#include <global.h>
#include <io/kio.h>
#include <types/nums.h>
#include <types/vargs.h>
/**
 * @brief Clears the screen by filling the video memory with spaces.
 *
 * Resets cursor position to the top-left corner (0,0).
 */
void Kcfp() {
    for (int i = 0; i < 80 * 25; i++) {
        vmem[i * 2] = ' ';
        vmem[(i * 2) + 1] = 0x0F;
    }
    col = 0;
    row = 0;
}

/**
 * @brief Outputs a single character to the screen at the current cursor
 * position.
 *
 * Supports newline ('\n') to move cursor to the next line.
 * Advances cursor position and scrolls screen if necessary.
 *
 * @param c Character to output
 */
void Kputchar(char c) {
    if (c == '\n') {
        col = 0;
        row++;
    } else {
        int index = (row * 80 + col) * 2;
        vmem[index] = c;
        vmem[index + 1] = 0x07;
        col++;
        if (col >= 80) {
            col = 0;
            row++;
        }
    }

    if (row >= 25) {
        Kcfp();
        row = 0;
    }
}

/**
 * @brief Handles backspace keypress by moving the cursor back and clearing the
 * character.
 */
void KputcharBackspace() {
    if (col > 0) {
        col--;
    } else if (row > 0) {
        row--;
        col = 79;
    }

    int offset = (row * 80) + col;
    vmem[offset * 2] = ' ';        // clear the character
    vmem[(offset * 2) + 1] = 0x07; // normal attribute
}

/**
 * @brief Prints a 32-bit unsigned integer in hexadecimal format prefixed with
 * "0x".
 *
 * Example output: 0xDEADBEEF
 *
 * @param num The number to print
 */
void KprintHex(u32 num) {
    char buf[11] = "0x00000000";
    const char *hex = "0123456789ABCDEF";
    for (int i = 0; i < 8; i++) {
        buf[9 - i] = hex[(num >> (i * 4)) & 0xF];
    }
    Kprintf(buf);
}

/**
 * @brief Prints a signed integer in decimal format.
 *
 * Handles zero and negative numbers correctly.
 *
 * @param num The number to print
 */
void KprintDec(int num) {
    char buf[12]; // enough for -2^31
    int i = 0;
    if (num == 0) {
        Kputchar('0');
        return;
    }
    if (num < 0) {
        Kputchar('-');
        num = -num;
    }
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    while (i--) {
        Kputchar(buf[i]);
    }
}

/**
 * @brief Prints a null-terminated string to the screen.
 *
 * @param str Pointer to the string to print
 */
void KprintStr(const char *str) {
    while (*str) {
        Kputchar(*str++);
    }
}

/**
 * @brief Formatted output to the screen.
 *
 * Supports format specifiers:
 * - %c : character
 * - %s : string
 * - %d : signed decimal integer
 * - %x : unsigned hexadecimal integer
 * - %% : literal percent sign
 *
 * @param fmt Format string
 * @param ... Variable arguments corresponding to format specifiers
 */
void Kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
            case 'c': {
                char pchar = (char)va_arg(args, int);
                Kputchar(pchar);
                break;
            }
            case 's': {
                const char *pstr = va_arg(args, const char *);
                KprintStr(pstr);
                break;
            }
            case 'd': {
                int num = va_arg(args, int);
                KprintDec(num);
                break;
            }
            case 'x': {
                u32 hex = va_arg(args, u32);
                KprintHex(hex);
                break;
            }
            case '%': {
                Kputchar('%');
                break;
            }
            default:
                Kputchar('%');
                Kputchar(*fmt);
                break;
            }
        } else {
            Kputchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}