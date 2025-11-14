#include <iostream>
#include "vm_app.h"
using namespace std;

// F —> dirty pages stay dirty even after being evicted and paged back in


int main() {
    char *p1 = (char *) vm_extend(); 
    p1[0] = 'X'; // causes write fault → dirty = true

    vm_yield(); // force context switch (eviction likely)

    vm_syslog(p1, 1); // read fault if p1 was evicted — should reload and leave dirty = false

    // The buggy pager will leave dirty = true even after this read-only access
    return 0;
}
