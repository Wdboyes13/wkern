#include <global.h>
#include <io/keyin.h>
#include <io/outb.h>
#include <io/printer.h>
#include <utils/ksleep.h>
unsigned char scancode_to_ascii(unsigned char scancode) {
    static const unsigned char keymap[256] = {
        0,   27,  '1', '2', '3', '4', '5',  '6', // 0x00 - 0x07
        '7', '8', '9', '0', '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't',
        'y', 'u', 'i', 'o', 'p', '[', ']',  '\n', 0,    'a', 's', // 0x10 - 0x17
        'd', 'f', 'g', 'h', 'j', 'k', 'l',  ';',  '\'', '`', 0,   '\\', 'z',
        'x', 'c', 'v', // 0x18 - 0x1F
        'b', 'n', 'm', ',', '.', '/', 0,    '*',  0,    ' ', 0,   0,    0,
        0,   0,   0, // 0x20 - 0x27
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,   0,   0,    0,
        0,   0,   0,   0,   0,   0,   0,    0,    0,    0,
    };

    return keymap[scancode];
}

unsigned char kgetkey() {
    unsigned char sccode = inb(0x60);
    while (1) {
        sccode = inb(0x60);
        if (!(sccode & 0x80)) {
            return scancode_to_ascii(sccode);
        }
    }
}

void kgetstr(char *str, int length) {
    int i = 0;

    while (i < length - 1) {
        unsigned char key = 0;
        while (!(key = kgetkey()))
            ;

        if (key == '\n') {
            str[i] = '\0';
            kputchar('\n');
            return;
        }

        if (key == '\b' || key == 0x7F) { // 0x7F is DEL on some layouts
            if (i > 0) {
                i--;
                str[i] = '\0';
                kputchar_backspace();
            }
            continue;
        }
        str[i++] = key;
        kputchar(key);
        ZZZ(30);
    }

    str[length - 1] = '\0';
}
void kflush() {
    while (inb(0x64) & 0x01) {
        inb(0x60);
    }
}