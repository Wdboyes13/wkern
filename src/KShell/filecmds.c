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
#include <fileio/fileio.h>
#include <io/kio.h>
#include <mem/kmem.h>

/**
 * @brief Delete a file from the FAT16 filesystem.
 *
 * Usage: `rm [filename] [ext]`
 *
 * @param argv Array of arguments. argv[1] is the filename, argv[2] is the extension.
 * @param argc Number of arguments.
 */
void Rm(const char *argv[], int argc) { // rm [filename] [ext]
    if (argc < 3) {
        Kprintf("Not enough args\nUsage: rm [filename] [ext]");
        return;
    }
    Fat16RemoveFile(argv[1], argv[2]);
}

/**
 * @brief Create a new empty file on the FAT16 filesystem.
 *
 * Usage: `mKfile [filename] [ext]`
 *
 * @param argv Array of arguments. argv[1] is the filename, argv[2] is the extension.
 * @param argc Number of arguments.
 */
void Mkf(const char *argv[], int argc) { // mKfile [filename] [ext]
    if (argc < 3) {
        Kprintf("Not enough args\nUsage: mKfile [filename] [ext]");
        return;
    }
    Mkfile(argv[1], argv[2]);
}

/**
 * @brief Write data to a file in the FAT16 filesystem from user input.
 *
 * Prompts the user to input text and writes it to the specified file.
 * Automatically appends an EOF marker (0x1A) at the end.
 *
 * Usage: `write [filename] [ext]`
 *
 * @param argv Array of arguments. argv[1] is the filename, argv[2] is the extension.
 * @param argc Number of arguments.
 */
void Writef(const char *argv[], int argc) { // write [filename] [ext]
    if (argc < 3) {
        Kprintf("Not enough args\nUsage: write [filename] [ext]");
        return;
    }
    char *data = (char *)Kmalloc(512, 8);
    if (!data) {
        Kprintf("\nMalloc Failed!\n");
        Kfree(data);
        return;
    }
    Kprintf("Start Writing Data:\n");
    Kgetstr(data, 500); // only read 500 max
    Kflush();

    int len = 0;
    while (len < 500 && data[len] != '\0') {
        len++;
    }

    // Add EOF marKer
    if (len + 5 < 512) {
        data[len] = 0x1A;
        len++;
        data[len] = '\0'; // Optional if your writer handles C-strings
    }

    Writefile(argv[1], argv[2], data, len + 1); // Use actual size
    Kfree(data);
}


/**
 * @brief Read and display contents of a file from the FAT16 filesystem.
 *
 * Usage: `read [filename] [ext]`
 *
 * @param argv Array of arguments. argv[1] is the filename, argv[2] is the extension.
 * @param argc Number of arguments.
 */
void Readf(const char *argv[], int argc) { // read [filename] [ext]
    if (argc < 3) {
        Kprintf("Not enough args\nUsage: read [filename] [ext]");
        return;
    }
    Fileconts(argv[1], argv[2]);
}