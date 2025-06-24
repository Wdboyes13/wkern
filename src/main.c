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
#include <mem/kmem.h>
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
void kernel_main() {
    all_idt(); // Do all IDT/GDT Setup

    u32 lba = find_fat16_partition(); // Look for a FAT16 Partition on ATA
    kprint_hex(lba);                  // Print LBA Address
    if (!lba) {                       // If an ATA FAT16 Partition was not found
        lba = 0;                      // Set LBA To 0 (Superfloppy)
    }
    if (!fat16_mount(lba)) {                   // Try mounting
        panic("Failed to mount FAT16 volume"); // If mount fails panic
    }

    kheap_init(); // Initialize Heap

    kcfp();
    virtnet_setup();
    kprintf("\nHello form WKern!\n");
    kprintf("Enter your name: ");
    kgetstr(name, 19);
    kputchar('\n');

    kprintf("Hello, %s\n", name);

    kflush();
    kprintf("Welcome!\n\nStarting...\n");
    ZZZ(5);
    sh();
}
