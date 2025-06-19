#include <io/kio.h>
#include <wex/stdwex.h>
void runwex(unsigned char *wexexecu) {
    int *FVALS = read_vals(wexexecu);
    int STS = FVALS[1];
    unsigned char ST[STS];
    for (int i = 0; i < STS; i++) {
        ST[i] = wexexecu[i + 2];
    }
    chkwex(ST, STS, smt, 5);
    exec(wexexecu, FVALS);
}