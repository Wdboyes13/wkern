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

#include <io/kio.h>
#include <wex/stdwex.h>

// Read First Values
// Reads 0x00 - 0x09
/**
 * @brief Read First Values of WEX Executable
 * @param exec - Executable in hexadecimal array format
 * @return Integer array containing executable data - ret[0]=Entry Point
 * ret[1]=Symbol Table Size
 */
int *read_vals(const unsigned char *exec) {
    static int epa;
    static int sts;
    static int fvals[2];

    epa = exec[0];
    sts = exec[1];
    fvals[0] = epa;
    fvals[1] = sts;

    return fvals;
}

/**
 * @brief Verify symbols in executable
 * @param SMT - Executables symbol table - Hex Array
 * @param SMT_LEN - Executable symbol table length - int
 * @param ST - Master Symbol Table
 * @param ST_LEN - Master Symbol Table Length
 * @return int - 0 if valid, 1 if invalid
 */
int chKwex(const unsigned char *smt, int smt_len, const unsigned char *st,
           int st_len) {
    for (int i = 0; i < smt_len; i++) {
        int valid = 0;
        for (int j = 0; j < st_len; j++) {
            if (smt[i] == st[j]) {
                valid = 1;
                break;
            }
        }
        if (!valid) {
            return 1; // Invalid symbol found
        }
    }
    return 0; // All symbols valid
}

/**
 * @brief Automatically checks and executes an executable
 * @param WEXEXECU - WEX Executable stored as Hexadecimal Array
 * @return Void Function - Does not return value
 */
void Runwex(unsigned char *wexexecu) {
    int *fvals = read_vals(wexexecu);
    int sts = fvals[1];
    unsigned char st[sts];
    for (int i = 0; i < sts; i++) {
        st[i] = wexexecu[i + 2];
    }
    chKwex(st, sts, smt, 5);
    Exec(wexexecu);
}