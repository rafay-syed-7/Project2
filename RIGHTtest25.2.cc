#include <iostream>
#include "vm_app.h"
using namespace std;

// B —> syslog only checks first page for residency


int main() {
    // extend two pages
    char *p1 = (char *) vm_extend(); // page 0
    char *p2 = (char *) vm_extend(); // page 1

    // write only to page 1
    p2[0] = 'X';

    // syslog that starts on p1 and spills into p2
    vm_syslog(p1 + (VM_PAGESIZE - 1), 2); // spans across two pages

}
