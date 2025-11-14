#include <iostream>
#include "vm_app.h"

using namespace std;

// Tries to catch S —> vm fault asserts if address is invalid 


int main() {
    char *p;
    p = (char *) vm_extend();
    p[8500] = 'h'; // out of bounds
    vm_syslog(p, 8500);
}