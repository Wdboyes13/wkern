#include <types/nums.h>
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