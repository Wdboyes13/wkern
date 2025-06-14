#include <utils/ksleep.h>
void ZZZ(int ms) {
    for (int i = 0; i < ms * 1000; i++) {
        for (volatile int j = 0; j < 1000; j++) {
            __asm__ volatile("nop");
        }
    }
}