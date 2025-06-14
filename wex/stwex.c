#include <io/printer.h>
#include <wex/chkst.h>
#include <wex/exec.h>
#include <wex/rdfv.h>
#include <wex/st.h>
void runwex(unsigned char *wexexecu) {
    int *FVALS = read_vals(wexexecu);
    // Put in table validation calls here
    exec(wexexecu, FVALS);
}