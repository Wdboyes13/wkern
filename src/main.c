/*
WKern - A Bare Metal OS / Kernel I am making (For Fun)
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

#include <KShell/shell.h>
#include <err/kerror.h>
#include <fileio/fileio.h>
#include <global.h>
#include <idt/idtirq.h>
#include <io/kio.h>
#include <types/bool.h>
#include <utils/util.h>
char name[20];
int row = 0;
int col = 0;
bool capson = false;
void kernel_main() {

    all_idt();

    u32 lba = find_fat16_partition();
    kprint_hex(lba);
    if (!lba) {
        lba = 0;
    }

    if (!fat16_mount(lba)) {
        panic("Failed to mount FAT16 volume");
    }

    kcfp();

    kprintf("\nHello form WKern!\n");
    kprintf("Enter your name: ");
    kgetstr(name, 19);
    kputchar('\n');
    kprintf("Hello, ");
    kprintf(name);
    kprintf("!\n");

    kflush();
    kprintf("Welcome!\n\nStarting...\n");
    ZZZ(5);
    sh();
}
