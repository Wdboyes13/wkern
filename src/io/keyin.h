#pragma once

char kgetkey();
unsigned char scancode_to_ascii(unsigned char scancode);
void kgetstr(char *str, int length);
void kflush();