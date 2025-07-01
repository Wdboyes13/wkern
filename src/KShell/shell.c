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

#include <KShell/filecmds.h>
#include <KShell/shell.h>
#include <fileio/fileio.h>
#include <global.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <net/virtnet.h>
#include <utils/util.h>
#include <wex/stdwex.h>

void sh() {
    while (1) {
        char *cmd = (char *)Kmalloc(128, 8);
        if (!cmd) {
            Kprintf("Malloc Failed");
            Kfree(cmd);
        } else {
            Kprintf("\nSH> ");
            Kgetstr(cmd, 127);
            Kflush();
            char *argv[8];
            int argc = Split(cmd, argv, 8);
            if (Kstrcmp(argv[0], "cmp") == 0) {
                Kshcmp();
            } else if (Kstrcmp(argv[0], "shutdown") == 0) {
#ifdef VMQEMU
#include <qemu/shutdown.h>
                QemuShutdown();
#endif
            } else if (Kstrcmp(argv[0], "help") == 0) {
                Help();
            } else if (Kstrcmp(argv[0], "tst") == 0) {
                Runwex(Execr());
            } else if (Kstrcmp(argv[0], "clear") == 0) {
                Kcfp();
            } else if (Kstrcmp(argv[0], "ls") == 0) {
                Fat16Ls();
            } else if (Kstrcmp(argv[0], "read") == 0) {
                Readf((const char **)argv, argc);
            } else if (Kstrcmp(argv[0], "user") == 0) {
                Kprintf("\nUsername: %s", name);
            } else if (Kstrcmp(argv[0], "mKfile") == 0) {
                Mkf((const char **)argv, argc);
            } else if (Kstrcmp(argv[0], "rm") == 0) {
                Rm((const char **)argv, argc);
            } else if (Kstrcmp(argv[0], "setname") == 0) {
                if (argc < 2) {
                    Kprintf("Not enough args\nUsage: setname [New Name]");
                    continue;
                }
                name = argv[1];
            } else if (Kstrcmp(argv[0], "write") == 0) {
                Writef((const char **)argv, argc);
            } else if (Kstrcmp(argv[0], "regex") == 0) {
                Regexc();
            } else if (Kstrcmp(argv[0], "recvpacK") == 0) {
                unsigned char Key = 0;
                while (!(Key = Kgetkey()) && Key == 'a') {
                    NetStq(iob);
                }
            } else {
                Kprintf("\nUnKnown Command - Try `help`\n");
            }
            Kfree(cmd);
        }
    }
}