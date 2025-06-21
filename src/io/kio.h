#pragma once
#include <types/nums.h>
void kprintf(const char *msg);
void kputchar(char c);
void kcfp();
void kputchar_backspace();
void kprint_hex(u32 num);
char kgetkey();
unsigned char scancode_to_ascii(unsigned char scancode);
void kgetstr(char *str, int length);
void kflush();
void outb(u16 port, u8 value);
u8 inb(u16 port);
void outw(u16 port, u16 value);
u16 inw(u16 port);
void cli();
void sti();