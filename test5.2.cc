#include <iostream>
#include "vm_app.h"
using namespace std;

//gives out of bounds length for syslog, syslog should return -1 and program shouldnt crash
int main() {
    char *p = (char *) vm_extend();
    p[0] = 'A';
    vm_syslog(p, 1000000); // absurdly long length, crosses arena boundary
}