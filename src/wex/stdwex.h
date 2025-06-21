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

#pragma once
static unsigned char smt[] = {
    0x0A, // Write
    0x0B, // Read
    0x0C, // Shutdown system
    0x0D, // Sleep 1
    0x01  // Clear Screen
};
unsigned char *execr();
void runwex(unsigned char *wexexecu);
int *read_vals(unsigned char *exec);
int chkwex(const unsigned char *smt, int smt_len, const unsigned char *st,
           int st_len);

void exec(unsigned char *exec, int *FVALS);