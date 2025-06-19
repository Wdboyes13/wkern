#include <wex/stdwex.h>
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