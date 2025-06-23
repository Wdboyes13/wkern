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

#include <KShell/filecmds.h>
#include <KShell/shell.h>
#include <fileio/fileio.h>
#include <global.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <utils/util.h>
#include <wex/stdwex.h>

void sh() {
    while (1) {
        char *cmd = kmalloc(128);
        kprintf("\nSH> ");
        kgetstr(cmd, 127);
        kflush();
        if (kstrcmp(cmd, "cmp") == 0) {
            kshcmp();
        } else if (kstrcmp(cmd, "shutdown") == 0) {
#ifdef VMQEMU
#include <qemu/shutdown.h>
            qemu_shutdown();
#endif
        } else if (kstrcmp(cmd, "help") == 0) {
            help();
        } else if (kstrcmp(cmd, "tst") == 0) {
            runwex(execr());
        } else if (kstrcmp(cmd, "clear") == 0) {
            kcfp();
        } else if (kstrcmp(cmd, "ls") == 0) {
            fat16_ls();
        } else if (kstrcmp(cmd, "read") == 0) {
            readf();
        } else if (kstrcmp(cmd, "user") == 0) {
            kputchar('\n');
            kprintf(name);
        } else if (kstrcmp(cmd, "mkfile") == 0) {
            mkf();
        } else if (kstrcmp(cmd, "rm") == 0) {
            rm();
        } else if (kstrcmp(cmd, "setname") == 0) {
            kprintf("Enter your name: ");
            kgetstr(name, 19);
            kputchar('\n');
        } else if (kstrcmp(cmd, "write") == 0) {
            writef();
        } else {
            kprintf("\nUnknown Command - Try `help`\n");
        }
        kfree(cmd);
    }
}