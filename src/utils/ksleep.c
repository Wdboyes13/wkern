#include <idt/idtirq.h>
#include <io/kio.h>
#include <types/nums.h>
#include <utils/util.h>
void ZZZ(int ms) { ms_sleep(ms); }
void SSleep(int secs) { ms_sleep(secs * 1000); }
