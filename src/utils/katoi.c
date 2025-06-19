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