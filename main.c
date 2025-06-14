#include <io/printer.h>
#include <types/nums.h>
void kernel_main(){
    kcfp();
    kprintf("Hello from kernel!\n");
    kprintf("This is a newline!!!\n");
    kuint32_t var = (kuint32_t)69420;
    char num[10];
    kitoa(var, num);
    kprintf(num);
}