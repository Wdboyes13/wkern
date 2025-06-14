// This is a test executable of the WEX format
// The data would be in an actual file normally
// This is just an array to simulate it
static unsigned char exec[] = {
    0x05, 0x02,       // Start exec at 0x05, symbol table length 02
    0x0A, 0x0C, 0x0D, // Print, Shutdown, Sleep 1
    0x0A, 'H',  'E',  'L', 'L', 'O', '\n', 0x00, // Call Code - Data - End Byte
    0x0D, 0x00, 0x0C, 0x00};
unsigned char *execr() { return exec; }