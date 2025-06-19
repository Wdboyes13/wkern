#include <io/kio.h>
#include <utils/util.h>
#include <wex/stdwex.h>
#ifdef VMQEMU
#include <qemu/shutdown.h>
#endif

extern unsigned char smt[];

void exec(unsigned char *exec, int *FVALS) {
    kprintf("Interpreting executable before execution");
    int entry = exec[0];
    int symtbl_len = exec[1];
    int pc = entry;
    while (exec[pc] != 0xFF) {
        unsigned char opcode = exec[pc++];

        if (opcode == 0x00) { // CALL
            unsigned char sym_id = exec[pc++];
            if (sym_id >= symtbl_len) {
                kprintf("Invalid symbol ID");
                break;
            }

            unsigned char op = smt[sym_id]; // Look up actual opcode

            switch (op) {
            case 0x0A: { // Write (print)
                while (exec[pc] != 0x00) {
                    kputchar(exec[pc++]);
                }
                pc++; // Skip 0x00
                break;
            }
            case 0x0B: {
                // Optional: implement read
                break;
            }
            case 0x0C: { // Shutdown
#ifdef VMQEMU
                qemu_shutdown();
#endif
                break;
            }
            case 0x0D: {
                ZZZ(1000);
                break;
            }
            case 0x01: {
                kcfp();
                break;
            }
            default: {
                kprintf("Unknown opcode");
                break;
            }
            }
        } else {
            kprintf("Invalid instruction byte\n");
            break;
        }
    }
}