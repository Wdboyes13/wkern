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
#include <io/kio.h>

void help() {
    kprintf("\nshutdown - shutdown system");
    kprintf("\nhelp - list command");
    kprintf("\ncmp - compare strings");
    kprintf("\nclear - clear screen");
    kprintf("\ntst - Test a WEX Executable");
    kprintf("\nls - List Files in Mounted FAT16");
    kprintf("\nread - Read File Contents from Cluster (512 Bytes)");
    kprintf("\nuser - List Username");
    kprintf("\nmkfile - Make a new file");
    kprintf("\nrm - Remove a file");
    kprintf("\nsetname - Set Username");
    kprintf("\nwrite - Write to a File");
}