#include <idt/idtirq.h>
#include <io/kio.h>
#include <types/nums.h>
volatile kuint32_t tick_count = 0;

void irq0_handler_c() {
    tick_count++;
    outb(0x20, 0x20); // PIC EOI
}

void ms_sleep(unsigned int ms) {
    unsigned int start = tick_count;
    while ((tick_count - start) < ms) {
        __asm__ volatile("hlt");
    }
}