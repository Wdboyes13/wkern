#include <io/printer.h>
#include <utils/img.h>
void draw_smile() {
    kputchar(0xDB);
    kprintf("        ");
    kputchar(0xDB);
    kputchar('\n');
    kputchar('\n');
    kputchar(0xDB);
    kprintf("        ");
    kputchar(0xDB);
    kputchar('\n');
    kputchar(' ');
    kputchar(0xDB);
    kputchar(0xDB);
    kputchar(0xDB);
    kputchar(0xDB);
    kputchar(0xDB);
    kputchar(0xDB);
    kputchar(0xDB);
    kputchar(0xDB);
    kputchar(' ');
    kputchar('\n');
}
