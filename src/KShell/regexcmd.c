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
#include <global.h>
#include <io/kio.h>
#include <mem/kmem.h>
#include <slre/slre.h>
#include <utils/util.h>

void regexc() {
    char regexp[100];
    char *res = (char *)kmalloc(300);

    if (!res) {
        kprintf("Malloc failed!\n");
        return;
    }

    kprintf("Enter regex expression: ");
    kgetstr(regexp, 99);

    kprintf("Enter text: ");
    kgetstr(res, 299);

    // Allocate space for capture groups (we'll use 1 group here)
    struct slre_cap caps[1];

    int matched = slre_match(regexp, res, kstrlen(res), caps, 1, 0);
    if (matched > 0) {
        kprintf("Match! Matched ");
        kprint_hex(matched);
        kputchar('\n');

        // Print matched substring
        int len = caps[0].len;
        kprintf("Matched text: ");
        for (int i = 0; i < len; i++) {
            kputchar(caps[0].ptr[i]);
        }
        kputchar('\n');
    } else {
        kprintf("No match.\n");
    }

    kfree(res);
}