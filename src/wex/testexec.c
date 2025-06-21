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

// This is a test executable of the WEX format
// The data would be in an actual file normally
// This is just an array to simulate it
static unsigned char exec[] = {
    0x06, 0x04,             // Entry point = 0x06, Symbol Table Size = 4
    0x0A, 0x0C, 0x0D, 0x01, // Symbol Table: [0]=Write, [1]=Shutdown, [2]=Sleep,
                            // [3]=CLS

    0x00, 0x03, // CALL CLS (smt[3] = 0x01)
    0x00, 0x00, 'H', 'E', 'L', 'L', 'O', '\n', 0x00, 0x00,
    0x02,       // CALL SLEEP (smt[2] = 0x0D)
    0x00, 0x01, // CALL SHUTDOWN (smt[1] = 0x0C)

    0xFF // End of program
};
unsigned char *execr() { return exec; }