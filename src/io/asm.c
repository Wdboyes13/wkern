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
#include <io/kio.h>
#include <types/nums.h>

#ifndef __clang__ // We do this to maKe ClangD shutup

/**
 * @brief Write a byte to the specified I/O port.
 *
 * @param port I/O port to write to
 * @param val Byte value to write
 */
inline void Outb(u16 port, u8 val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * @brief Read a byte from the specified I/O port.
 *
 * @param port I/O port to read from
 * @return Byte value read
 */
inline u8 Inb(u16 port) {
    u8 ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Write a 16-bit word to the specified I/O port.
 *
 * @param port I/O port to write to
 * @param val 16-bit value to write
 */
inline void Outw(u16 port, u16 val) {
    __asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * @brief Read a 16-bit word from the specified I/O port.
 *
 * @param port I/O port to read from
 * @return 16-bit value read
 */
inline u16 Inw(u16 port) {
    u16 ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Write a 32-bit value to the specified I/O port.
 *
 * @param port I/O port to write to
 * @param val 32-bit value to write
 */
inline void Outl(u16 port, u32 val) {
    __asm__ volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}

/**
 * @brief Read a 32-bit value from the specified I/O port.
 *
 * @param port I/O port to read from
 * @return 32-bit value read
 */
inline u32 Inl(u16 port) {
    u32 ret;
    __asm__ volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/**
 * @brief Disable CPU interrupts (clear interrupt flag).
 *
 * This is often used before critical sections to prevent
 * preemption by interrupt handlers.
 */
inline void Cli() {
    __asm__ volatile("cli");
}

/**
 * @brief Enable CPU interrupts (set interrupt flag).
 *
 * This is used after a critical section to re-enable interrupts.
 * NOPs are added for minor instruction delay.
 */
inline void Sti() {
    __asm__ volatile("sti; nop; nop; nop");
}

#endif