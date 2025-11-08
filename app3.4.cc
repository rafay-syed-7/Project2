#include <iostream>
#include "vm_app.h"

using namespace std;

//makes sure each process has its own arena and disk space
int main() {
    char *p = (char *) vm_extend();
    p[0] = 'A';
    vm_syslog(p, 1); //Process 1 is 'A'

    vm_yield(); 
    char *q = (char *) vm_extend();
    q[0] = 'B';
    vm_syslog(q, 1); //Process 2 is 'B'

    vm_yield(); 

    char *r = (char *) vm_extend();
    r[0] = 'C';
    vm_syslog(r, 1); //Process 3 is 'C'
}