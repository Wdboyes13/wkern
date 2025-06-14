
#include "global.h"

void kcfp() {
    for (int i = 0; i < 80 * 25; i++) {
        vmem[i * 2] = ' ';
        vmem[i * 2 + 1] = 0x0F;
    }
}

void kputchar(char c) {
    if (c == '\n') {
        col = 0;
        row++;
    } else {
        int index = (row * 80 + col) * 2;
        vmem[index] = c;
        vmem[index + 1] = 0x07;
        col++;
        if (col >= 80) {
            col = 0;
            row++;
        }
    }

    if (row >= 25) {
        kcfp();
        row = 0;
    }
}

void kputchar_backspace() {
    if (col > 0) {
        col--;
    } else if (row > 0) {
        row--;
        col = 79;
    }

    int offset = row * 80 + col;
    vmem[offset * 2] = ' ';      // clear the character
    vmem[offset * 2 + 1] = 0x07; // normal attribute
}

void kprintf(const char *msg) {
    for (int i = 0; msg[i] != 0; i++) {
        kputchar(msg[i]);
    }
}