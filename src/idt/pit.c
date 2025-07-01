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

#include <io/kio.h>
#include <types/nums.h>
// PIT ports
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND 0x43

// Command byte bits for mode 3 (square wave generator)
#define PIT_ACCESS_LOHI 0x30 // access low byte then high byte
#define PIT_MODE3 0x06       // mode 3 (square wave)

// PIT frequency and divisor
#define PIT_FREQ 1193182 // base frequency in Hz
// Setup PIT for a given frequency (e.g., 100 Hz)
void PitInit(u32 freq) {
    u16 divisor = (u16)(PIT_FREQ / freq);
    Kprintf("Divisor Set\n");
    // Send command byte: channel 0, access mode lo/hi, mode 3, binary mode
    Outb(PIT_COMMAND, PIT_ACCESS_LOHI | PIT_MODE3);
    Kprintf("Command Byte Sent\n");
    // Send divisor low byte
    Outb(PIT_CHANNEL0, divisor & 0xFF);
    Kprintf("Low byte sent\n");
    // Send divisor high byte
    Outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
    Kprintf("High byte sent\n");
}