#include <io/kio.h>
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

inline kuint16_t inw(kuint16_t port) {
    kuint16_t ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

inline void cli() { __asm__ volatile("cli"); }
inline void sti() { __asm__ volatile("sti; nop; nop; nop"); }