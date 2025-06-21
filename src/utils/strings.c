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

#include <io/kio.h>
#include <utils/util.h>
int kstrcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

void kshcmp() {

    kprintf("\nSTR1>");
    char str1[256];
    kgetstr(str1, sizeof(str1));
    kflush();
    kprintf("\nSTR2>");
    char str2[256];
    kgetstr(str2, sizeof(str2));
    kflush();
    if (kstrcmp(str1, str2) == 0) {
        kprintf("\nEqual");
    } else {
        kprintf("\nNot Equal");
    }
}

u32 kmstrlen(const char *str) {
    u32 len = 0;
    while (str[len])
        len++;
    return len;
}