#include <iostream>
#include "vm_app.h"
using namespace std;

//Tests write faults
int main() {
    char *p = (char *) vm_extend();
    p[0] = 'X';
    vm_syslog(p, 1);
}