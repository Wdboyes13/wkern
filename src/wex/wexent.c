#include <io/kio.h>
#include <wex/stdwex.h>

// Read First Values
// Reads 0x00 - 0x09
int *read_vals(unsigned char *exec) {
    static int EPA;
    static int STS;
    static int FVALS[2];

    EPA = exec[0];
    STS = exec[1];
    FVALS[0] = EPA;
    FVALS[1] = STS;

    return FVALS;
}

// Check if every symbol in `smt` exists in `st`
int chkwex(const unsigned char *smt, int smt_len, const unsigned char *st,
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