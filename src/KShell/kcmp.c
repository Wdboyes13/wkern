#include <KShell/shell.h>
#include <io/keyin.h>
#include <io/printer.h>
#include <utils/kstrcmp.h>

void kcmp() {

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