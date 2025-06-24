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

char *kstrchr(const char *str, int c) {
    while (*str) {
        if (*str == (char)c) {
            return (char *)str;
        }
        str++;
    }
    if ((char)c == '\0') {
        return (char *)str;
    }
    return NULL;
}

int ktolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A'); // Shift from uppercase to lowercase
    }
    return c; // Not uppercase, just return as is
}

// Check if the char is a digit (0-9)
int kisdigit(int c) { return (c >= '0' && c <= '9'); }

int kisspace(int c) {
    return (c == ' ' ||  // space
            c == '\t' || // horizontal tab
            c == '\n' || // newline
            c == '\v' || // vertical tab
            c == '\f' || // form feed
            c == '\r');  // carriage return
}

size_t kstrlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int kisxdigit(int c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

int split(char *input, char **argv, int max_args) {
    int argc = 0;

    while (*input && argc < max_args) {
        // Skip leading spaces
        while (*input == ' ')
            input++;

        if (*input == '\0')
            break;

        // Mark the beginning of the argument
        argv[argc++] = input;

        // Find the end of the argument
        while (*input && *input != ' ')
            input++;

        if (*input == ' ') {
            *input = '\0'; // Null-terminate the string
            input++;       // Move to the next character
        }
    }

    return argc;
}