#pragma once
#include <types/nums.h>
void kprintf(const char *msg);
void kputchar(char c);
void kcfp();
void kputchar_backspace();
void kprint_hex(kuint32_t num);
char kgetkey();
unsigned char scancode_to_ascii(unsigned char scancode);
void kgetstr(char *str, int length);
void kflush();
void outb(kuint16_t port, kuint8_t value);
kuint8_t inb(kuint16_t port);
void outw(kuint16_t port, kuint16_t value);
kuint16_t inw(kuint16_t port);
void cli();
void sti();