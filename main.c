#include "printer.h"
void kernel_main(){
    kcfp();
    kprintf("Hello from kernel!\n");
    kprintf("This is a newline!!!\n");
}