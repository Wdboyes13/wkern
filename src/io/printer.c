/*
WKern - A Bare Metal OS / Kernel I am making (For Fun)
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
void kcfp() {
    for (int i = 0; i < 80 * 25; i++) {
        vmem[i * 2] = ' ';
        vmem[i * 2 + 1] = 0x0F;
    }
    col = 0;
    row = 0;
}

void kputchar(char c) {
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
        kcfp();
        row = 0;
    }
}

void kputchar_backspace() {
    if (col > 0) {
        col--;
    } else if (row > 0) {
        row--;
        col = 79;
    }

    int offset = row * 80 + col;
    vmem[offset * 2] = ' ';      // clear the character
    vmem[offset * 2 + 1] = 0x07; // normal attribute
}

void kprint_hex(u32 num) {
    char buf[11] = "0x00000000";
    const char *hex = "0123456789ABCDEF";
    for (int i = 0; i < 8; i++) {
        buf[9 - i] = hex[(num >> (i * 4)) & 0xF];
    }
    kprintf(buf);
}

void kprint_dec(int num) {
    char buf[12]; // enough for -2^31
    int i = 0;
    if (num == 0) {
        kputchar('0');
        return;
    }
    if (num < 0) {
        kputchar('-');
        num = -num;
    }
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    while (i--)
        kputchar(buf[i]);
}

void kprint_str(const char *str) {
    while (*str) {
        kputchar(*str++);
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
            case 'c': {
                char c = (char)va_arg(args, int);
                kputchar(c);
                break;
            }
            case 's': {
                const char *s = va_arg(args, const char *);
                kprint_str(s);
                break;
            }
            case 'd': {
                int num = va_arg(args, int);
                kprint_dec(num);
                break;
            }
            case 'x': {
                u32 hex = va_arg(args, u32);
                kprint_hex(hex);
                break;
            }
            case '%': {
                kputchar('%');
                break;
            }
            default:
                kputchar('%');
                kputchar(*fmt);
                break;
            }
        } else {
            kputchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}