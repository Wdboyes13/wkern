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

#include <utils/util.h>
int katoi(const char *str) {
    int result = 0;
    int sign = 1;
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t') {
        i++;
    }
    if (str[i] == '-') {
        sign -= 1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9') {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return result * sign;
}

int katoi_auto(const char *str) {
    if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
        // Hex string
        int val = 0;
        str += 2; // skip "0x"
        while (*str) {
            val *= 16;
            if (*str >= '0' && *str <= '9')
                val += *str - '0';
            else if (*str >= 'a' && *str <= 'f')
                val += *str - 'a' + 10;
            else if (*str >= 'A' && *str <= 'F')
                val += *str - 'A' + 10;
            else
                break;
            str++;
        }
        return val;
    } else {
        // Decimal fallback
        return katoi(str);
    }
}

void kitoa(unsigned int num, char *buf) {
    int i = 0;
    if (num == 0) {
        buf[i++] = '0';
    } else {
        // Convert digits backwards
        while (num > 0) {
            buf[i++] = '0' + (num % 10);
            num /= 10;
        }
    }
    buf[i] = '\0';

    // Reverse string
    for (int j = 0; j < i / 2; j++) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }
}