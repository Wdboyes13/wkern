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
        char *cmd = (char *)kmalloc(128);
        if (!cmd) {
            kprintf("Malloc Failed");
            kfree(cmd);
        } else {
            kprintf("\nSH> ");
            kgetstr(cmd, 127);
            kflush();
            char *argv[8];
            int argc = split(cmd, argv, 8);
            if (kstrcmp(argv[0], "cmp") == 0) {
                kshcmp();
            } else if (kstrcmp(argv[0], "shutdown") == 0) {
#ifdef VMQEMU
#include <qemu/shutdown.h>
                qemu_shutdown();
#endif
            } else if (kstrcmp(argv[0], "help") == 0) {
                help();
            } else if (kstrcmp(argv[0], "tst") == 0) {
                runwex(execr());
            } else if (kstrcmp(argv[0], "clear") == 0) {
                kcfp();
            } else if (kstrcmp(argv[0], "ls") == 0) {
                fat16_ls();
            } else if (kstrcmp(argv[0], "read") == 0) {
                readf((const char **)argv, argc);
            } else if (kstrcmp(argv[0], "user") == 0) {
                kprintf("\nUsername: %s", name);
            } else if (kstrcmp(argv[0], "mkfile") == 0) {
                mkf((const char **)argv, argc);
            } else if (kstrcmp(argv[0], "rm") == 0) {
                rm((const char **)argv, argc);
            } else if (kstrcmp(argv[0], "setname") == 0) {
                if (argc < 2) {
                    kprintf("Not enough args\nUsage: setname [New Name]");
                    continue;
                }
                name = argv[1];
            } else if (kstrcmp(argv[0], "write") == 0) {
                writef((const char **)argv, argc);
            } else if (kstrcmp(argv[0], "regex") == 0) {
                regexc();
            } else {
                kprintf("\nUnknown Command - Try `help`\n");
            }
            kfree(cmd);
        }
    }
}