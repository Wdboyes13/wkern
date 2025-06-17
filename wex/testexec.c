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