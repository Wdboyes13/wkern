#include <io/printer.h>
#include <ldints.h>
extern void _load_interrupts(void);

void ldints(void) {
    kprintf("Enabling Interrupts");
    _load_interrupts();
    kprintf("Enabled interrupts");
}