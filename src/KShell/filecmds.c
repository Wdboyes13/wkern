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
#include <fileio/fileio.h>
#include <io/kio.h>
#include <mem/kmem.h>
void rm() {
    char filename[9];
    char filext[4];
    kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
    kgetstr(filename, 9);
    kflush();
    kputchar('\n');
    kprintf("Enter File Extension: ");
    kgetstr(filext, 4);
    kflush();
    kputchar('\n');
    fat16_remove_file(filename, filext);
}

void mkf() {
    char filename[9];
    char filext[4];
    kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
    kgetstr(filename, 9);
    kflush();
    kputchar('\n');
    kprintf("Enter File Extension: ");
    kgetstr(filext, 4);
    kflush();
    kputchar('\n');
    mkfile(filename, filext);
}

void writef() {
    char filename[9] = {0};
    char ext[4] = {0};
    char *data = (char *)kmalloc(512);
    if (!data) {
        kprintf("\nMalloc Failed!\n");
        kfree(data);
        return;
    }
    kprintf("\nEnter Filename (Without Extension): ");
    kgetstr(filename, 9);
    kflush();
    kputchar('\n');

    kprintf("Enter File Extension: ");
    kgetstr(ext, 4);
    kflush();
    kputchar('\n');

    kprintf("Start Writing Data:\n");
    kgetstr(data, 500); // only read 500 max
    kflush();

    int len = 0;
    while (len < 500 && data[len] != '\0') {
        len++;
    }

    // Add EOF marker
    if (len + 5 < 512) {
        data[len] = 0x1A;
        len++;
        data[len] = '\0'; // Optional if your writer handles C-strings
    }

    writefile(filename, ext, data, len + 1); // Use actual size
    kfree(data);
}

void readf() {
    char filename[9];
    char filext[4];
    kprintf("\nEnter Filename 7 Letters Max (Without Extension): ");
    kgetstr(filename, 9);
    kflush();
    kputchar('\n');
    kprintf("Enter File Extension: ");
    kgetstr(filext, 4);
    kflush();
    kputchar('\n');
    fileconts(filename, filext);
}