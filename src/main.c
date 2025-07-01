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

#include <KShell/shell.h>
#include <err/Kerror.h>
#include <fileio/fileio.h>
#include <global.h>
#include <idt/idtirq.h>
#include <io/Kio.h>
#include <mem/Kmem.h>
#include <net/virtnet.h>
#include <pci/pci.h>
#include <types/bool.h>
#include <utils/util.h>

char *name;
int row = 0;
int col = 0;
bool capson = false;
bool shift = false;
u32 iob = 0;
void KernelMain() {
    AllIdt(); // Do all IDT/GDT Setup

    u32 lba = FindFat16Partition(); // LooK for a FAT16 Partition on ATA
    KprintHex(lba);                 // Print LBA Address
    if (!lba) {                     // If an ATA FAT16 Partition was not found
        lba = 0;                    // Set LBA To 0 (Superfloppy)
    }
    if (!Fat16Mount(lba)) {                    // Try mounting
        Panic("Failed to mount FAT16 volume"); // If mount fails panic
    }

    KheapInit(); // Initialize Heap

    Kcfp();
    VirtnetSetup();
    Kprintf("\nHello form WKern!\n");
    Kprintf("Enter your name: ");
    Kgetstr(name, 19);
    Kputchar('\n');

    Kprintf("Hello, %s\n", name);

    Kflush();
    Kprintf("Welcome!\n\nStarting...\n");
    ZZZ(5);
    Sh();
}
