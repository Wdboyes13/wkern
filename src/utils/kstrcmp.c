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