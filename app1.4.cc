#include <iostream>
#include "vm_app.h"
using namespace std;

//Test extend and syslog
int main() {
    char *p = (char *) vm_extend();
    p[0] = 'H';
    p[1] = 'i';
    vm_syslog(p, 2);
}