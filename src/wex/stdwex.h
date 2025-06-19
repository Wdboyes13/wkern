#pragma once
static unsigned char smt[] = {
    0x0A, // Write
    0x0B, // Read
    0x0C, // Shutdown system
    0x0D, // Sleep 1
    0x01  // Clear Screen
};
unsigned char *execr();
void runwex(unsigned char *wexexecu);
int *read_vals(unsigned char *exec);
int chkwex(const unsigned char *smt, int smt_len, const unsigned char *st,
           int st_len);

void exec(unsigned char *exec, int *FVALS);