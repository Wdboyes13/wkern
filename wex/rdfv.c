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