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
#include <global.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <slre/slre.h>
#include <utils/util.h>

void Regexc() {
    char regexp[100];
    char *res = (char *)Kmalloc(300, 8);

    if (!res) {
        Kprintf("Malloc failed!\n");
        return;
    }

    Kprintf("Enter regex expression: ");
    Kgetstr(regexp, 99);

    Kprintf("Enter text: ");
    Kgetstr(res, 299);

    // Allocate space for capture groups (we'll use 1 group here)
    struct slre_cap caps[1];

    int matched = slre_match(regexp, res, Kstrlen(res), caps, 1, 0);
    if (matched > 0) {
        Kprintf("Match! Matched ");
        KprintHex(matched);
        Kputchar('\n');

        // Print matched substring
        int len = caps[0].len;
        Kprintf("Matched text: ");
        for (int i = 0; i < len; i++) {
            Kputchar(caps[0].ptr[i]);
        }
        Kputchar('\n');
    } else {
        Kprintf("No match.\n");
    }

    Kfree(res);
}