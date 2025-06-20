#include <io/kio.h>
#include <types/nums.h>
// PIT ports
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND 0x43

// Command byte bits for mode 3 (square wave generator)
#define PIT_ACCESS_LOHI 0x30 // access low byte then high byte
#define PIT_MODE3 0x06       // mode 3 (square wave)

// PIT frequency and divisor
#define PIT_FREQ 1193182 // base frequency in Hz
// Setup PIT for a given frequency (e.g., 100 Hz)
void pit_init(kuint32_t freq) {
    kuint16_t divisor = (kuint16_t)(PIT_FREQ / freq);
    kprintf("Divisor Set\n");
    // Send command byte: channel 0, access mode lo/hi, mode 3, binary mode
    outb(PIT_COMMAND, PIT_ACCESS_LOHI | PIT_MODE3);
    kprintf("Command Byte Sent\n");
    // Send divisor low byte
    outb(PIT_CHANNEL0, divisor & 0xFF);
    kprintf("Low byte sent\n");
    // Send divisor high byte
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
    kprintf("High byte sent\n");
}