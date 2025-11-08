#include <iostream>
#include "vm_app.h"

using namespace std;

// tests what happens after calling extend aftery destroy
// should not crash 

int main() {
    char *p = (char *) vm_extend();
    p[0] = '1';
    p[1] = '2';
    vm_syslog(p, 2);

    vm_destroy(); 

    // Attempt another extend after destruction
    char *q = (char *) vm_extend();
    vm_syslog((char *)"extend after destroy", 21);
}