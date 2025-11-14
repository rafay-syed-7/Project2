#include <iostream>
#include "vm_app.h"
using namespace std;

// T —> vm fault makes a !referenced page read only on read fault, even if page was already dirty

int main() {
    char *p = (char *) vm_extend();
    p[0] = 'M'; // write → page is dirty

    vm_yield(); // switch out to trigger eviction

    // fault it back in via read
    char x = p[0];

    p[1] = 'Z'; // if vm_fault incorrectly made the page read-only, this write should fault again
    return 0;
}
