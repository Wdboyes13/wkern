#include <utils/katoi.h>
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