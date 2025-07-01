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
#include <utils/util.h>
#include <wex/stdwex.h>
#ifdef VMQEMU
#include <qemu/shutdown.h>
#endif

void Exec(unsigned char *exec, int *FVALS) {
    Kprintf("Interpreting executable before execution");
    int entry = exec[0];
    int symtbl_len = exec[1];
    int pc = entry;
    while (exec[pc] != 0xFF) {
        unsigned char opcode = exec[pc++];

        if (opcode == 0x00) { // CALL
            unsigned char sym_id = exec[pc++];
            if (sym_id >= symtbl_len) {
                Kprintf("Invalid symbol ID");
                break;
            }

            unsigned char op = smt[sym_id]; // LooK up actual opcode

            switch (op) {
            case 0x0A: { // Write (print)
                while (exec[pc] != 0x00) {
                    Kputchar(exec[pc++]);
                }
                pc++; // SKip 0x00
                break;
            }
            case 0x0B: {
                // Optional: implement read
                break;
            }
            case 0x0C: { // Shutdown
#ifdef VMQEMU
                QemuShutdown();
#endif
                break;
            }
            case 0x0D: {
                ZZZ(100);
                break;
            }
            case 0x01: {
                Kcfp();
                break;
            }
            default: {
                Kprintf("UnKnown opcode");
                break;
            }
            }
        } else {
            Kprintf("Invalid instruction byte\n");
            break;
        }
    }
}