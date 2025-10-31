#include <iostream>
#include "vm_app.h"

using namespace std;

// JEANNIE's Sample.cc file

int main() {
    char *p;
    p = (char *) vm_extend();
    p[0] = 'h';
    p[1] = 'e';
    p[2] = 'l';
    p[3] = 'l';
    p[4] = 'o';
    vm_syslog(p, 5);
}