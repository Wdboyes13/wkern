#include <io/asm.h>
#include <types/nums.h>
inline void outb(kuint16_t port, kuint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

inline kuint8_t inb(kuint16_t port) {
    kuint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

inline void outw(kuint16_t port, kuint16_t val) {
    __asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

inline void cli() { __asm__ volatile("cli"); }
inline void sti() {
    outb(0x21, inb(0x21) | 0x01);
    __asm__ volatile("sti; nop; nop; nop");
    outb(0x21, inb(0x21) & ~0x01);
}