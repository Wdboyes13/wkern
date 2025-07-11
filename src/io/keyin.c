/*
WKern - A Bare Metal OS / Kernel I am maKing (For Fun)
Copyright (C) 2025  Wdboyes13

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/**
 * @file keyboard.c
 * @brief PS/2 Keyboard input handling for WKern
 *
 * Handles scancode translation, buffering, and character input from a standard
 * PS/2 keyboard.
 */

#include <global.h>
#include <io/kio.h>

/// @brief Converts a keyboard scancode to an ASCII character.
/// @param scancode The raw scancode from keyboard hardware.
/// @return Corresponding ASCII character, or 0 if unmapped.
unsigned char scancode_to_ascii(unsigned char scancode) {
    static const unsigned char Keymap[256] = {
        0,   27,  '1', '2', '3', '4', '5',  '6', // 0x00 - 0x07
        '7', '8', '9', '0', '-', '=', '\b', '\t', 'q',  'w', 'e', 'r',  't',
        'y', 'u', 'i', 'o', 'p', '[', ']',  '\n', 0,    'a', 's', // 0x10 - 0x17
        'd', 'f', 'g', 'h', 'j', 'K', 'l',  ';',  '\'', '`', 0,   '\\', 'z',
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

    return Keymap[scancode];
}

/// Circular buffer storing incoming keystrokes.
volatile char Keybuf[256];
/// Head index for Keybuf
volatile int buf_head = 0;
/// Tail index for Keybuf
volatile int buf_tail = 0;

/// @brief Converts an ASCII character to its shifted equivalent.
/// @param ascii The base character
/// @return Shifted version of the input character, or the same character if no
/// shift mapping exists.
unsigned char Keytoshift(unsigned char ascii) {
    if (ascii >= 'a' && ascii <= 'z') {
        return ascii - 32; // 'a' to 'A'
    }

    switch (ascii) {
    case '1':
        return '!';
    case '2':
        return '@';
    case '3':
        return '#';
    case '4':
        return '$';
    case '5':
        return '%';
    case '6':
        return '^';
    case '7':
        return '&';
    case '8':
        return '*';
    case '9':
        return '(';
    case '0':
        return ')';
    case '-':
        return '_';
    case '=':
        return '+';
    case '[':
        return '{';
    case ']':
        return '}';
    case '\\':
        return '|';
    case ';':
        return ':';
    case '\'':
        return '"';
    case ',':
        return '<';
    case '.':
        return '>';
    case '/':
        return '?';
    case '`':
        return '~';
    default:
        return ascii; // default if no shift mapping exists
    }
}

/// @brief IRQ handler for PS/2 keyboard (IRQ1).
///
/// Reads raw scancode, handles modifier keys (Shift, CapsLock),
/// converts to ASCII, and stores in the `Keybuf` ring buffer.
void Irq1HandlerC(void) {
    unsigned char sc = Inb(0x60);

    if (!(sc & 0x80)) { // Key press (not release)
        if (sc == 0x3A) {
            capson = !capson;
            return;
        }

        if (sc == 0x2A || sc == 0x36) {
            shift = 1;
            return;
        }

        unsigned char ascii = scancode_to_ascii(sc);

        if (shift) {
            ascii = Keytoshift(ascii);
        }
        if (capson && ascii >= 'a' && ascii <= 'z') {
            ascii -= 32;
        }
        if (ascii == 0 || ascii > 127) {
            return;
        }
        Keybuf[buf_head++] = ascii;
        if (buf_head == 256) {
            buf_head = 0;
        }
    } else {
        // Key release
        if (sc == 0xAA || sc == 0xB6) {
            shift = 0;
        }
    }
}

/// @brief Gets the next character from the keyboard buffer (blocking).
/// @return The next ASCII character from the input buffer.
char Kgetkey() {
    while (buf_tail == buf_head) {
        ; // wait for Key
    }
    char ch = Keybuf[buf_tail++];
    if (buf_tail == 256) {
        buf_tail = 0;
    }
    return ch;
}

/// @brief Reads a line of input from the keyboard (blocking).
///
/// Handles basic line editing (Enter, Backspace, Delete).
///
/// @param str Destination buffer
/// @param length Maximum buffer length (including null terminator)
void Kgetstr(char *str, int length) {
    int i = 0;

    while (i < length - 1) {
        unsigned char Key = 0;
        while (!(Key = Kgetkey())) {
            ;
        }

        if (Key == '\n') {
            str[i] = '\0';
            Kputchar('\n');
            return;
        }

        if (Key == '\b' || Key == 0x7F) { // 0x7F is DEL on some layouts
            if (i > 0) {
                i--;
                str[i] = '\0';
                KputcharBackspace();
            }
            continue;
        }
        str[i++] = Key;
        Kputchar(Key);
    }

    str[length - 1] = '\0';
}

/// @brief Flushes the keyboard controller input buffer.
///
/// Reads and discards all unread bytes in the controller’s 0x60 port.
void Kflush() {
    while (Inb(0x64) & 0x01) {
        Inb(0x60);
    }
}