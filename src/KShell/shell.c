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

/**
 * @brief Command function pointer type.
 *
 * All shell commands must follow this function signature.
 *
 * @param argv Argument list
 * @param argc Argument count
 */
typedef void (*CommandFunc)(const char *argv[], int argc);

/**
 * @brief Represents an individual shell command.
 */
typedef struct {
    const char *name;
    CommandFunc func;
} CommandEntry;

/// @brief Compares two strings using the internal shell comparison function.
void CmdCmp(const char *argv[], int argc) { Kshcmp(); }
/// @brief Displays built-in help information.
void CmdHelp(const char *argv[], int argc) { Help(); }
/**
 * @brief Executes a test WEX executable.
 */
void CmdTst(const char *argv[], int argc) { Runwex(Execr()); }

/**
 * @brief Clears the shell screen.
 */
void CmdClear(const char *argv[], int argc) { Kcfp(); }
/**
 * @brief Lists FAT16 filesystem contents.
 */
void CmdLs(const char *argv[], int argc) { Fat16Ls(); }

/**
 * @brief Prints the current username.
 */
void CmdUser(const char *argv[], int argc) { Kprintf("\nUsername: %s", name); }
/**
 * @brief Runs the regex test utility.
 */
void CmdRegex(const char *argv[], int argc) { Regexc(); }
/**
 * @brief Receives network packets from VirtNet (on keypress).
 */
void CmdRecvPack(const char *argv[], int argc) {
    unsigned char Key = 0;
    while (!(Key = Kgetkey()) && Key == 'a') {
        NetStq(iob);
    }
}
/**
 * @brief Sends shutdown signal when running under QEMU.
 */
void CmdShutdown(const char *argv[], int argc) {
#ifdef VMQEMU
#include <qemu/shutdown.h>
    QemuShutdown();
#endif
}

/**
 * @brief Sets the username used in the shell.
 *
 * @param argv [0] = "setname", [1] = new name
 * @param argc Must be >= 2
 */
void CmdSetName(const char *argv[], int argc) {
    if (argc < 2) {
        Kprintf("Not enough args\nUsage: setname [New Name]");
        return;
    }
    name = argv[1];
}

/**
 * @brief Array of all available shell commands.
 */
CommandEntry commands[] = {
    {"cmp", CmdCmp},
    {"help", CmdHelp},
    {"tst", CmdTst},
    {"clear", CmdClear},
    {"ls", CmdLs},
    {"read", Readf},
    {"user", CmdUser},
    {"mKfile", Mkf},
    {"rm", Rm},
    {"write", Writef},
    {"regex", CmdRegex},
    {"recvpacK", CmdRecvPack},
    {"shutdown", CmdShutdown},
    {"setname", CmdSetName},
};

/**
 * @brief Number of commands available in the shell.
 */
#define NUM_COMMANDS (sizeof(commands) / sizeof(CommandEntry))

/**
 * @brief The main interactive WKern shell loop.
 *
 * This loop:
 * - Prompts the user for input
 * - Tokenizes input into arguments
 * - Looks up matching command in the command table
 * - Executes the corresponding command
 *
 * On failure, prints an unknown command message.
 *
 * @note This loop never exits.
 */
void Sh() {
    while (1) {
        char *cmd = (char *)Kmalloc(128, 8);
        if (!cmd) {
            Kprintf("Malloc Failed");
            Kfree(cmd);
            continue;
        }

        Kprintf("\nSH> ");
        Kgetstr(cmd, 127);
        Kflush();

        char *argv[8];
        int argc = Split(cmd, argv, 8);

        if (argc == 0) {
            Kfree(cmd);
            continue;
        }

        int found = 0;
        for (int i = 0; i < NUM_COMMANDS; i++) {
            if (Kstrcmp(argv[0], commands[i].name) == 0) {
                commands[i].func((const char **)argv, argc);
                found = 1;
                break;
            }
        }

        if (!found) {
            Kprintf("\nUnKnown Command - Try `help`\n");
        }

        Kfree(cmd);
    }
}