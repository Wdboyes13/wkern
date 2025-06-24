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
void rm(const char *argv[], int argc) { // rm [filename] [ext]
    if (argc < 3) {
        kprintf("Not enough args\nUsage: rm [filename] [ext]");
        return;
    }
    fat16_remove_file(argv[1], argv[2]);
}

void mkf(const char *argv[], int argc) { // mkfile [filename] [ext]
    if (argc < 3) {
        kprintf("Not enough args\nUsage: mkfile [filename] [ext]");
        return;
    }
    mkfile(argv[1], argv[2]);
}

void writef(const char *argv[], int argc) { // write [filename] [ext]
    if (argc < 3) {
        kprintf("Not enough args\nUsage: write [filename] [ext]");
        return;
    }
    char *data = (char *)kmalloc(512, 8);
    if (!data) {
        kprintf("\nMalloc Failed!\n");
        kfree(data);
        return;
    }
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

    writefile(argv[1], argv[2], data, len + 1); // Use actual size
    kfree(data);
}

void readf(const char *argv[], int argc) { // read [filename] [ext]
    if (argc < 3) {
        kprintf("Not enough args\nUsage: read [filename] [ext]");
        return;
    }
    fileconts(argv[1], argv[2]);
}