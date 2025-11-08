#include <iostream>
#include "vm_app.h"
using namespace std;

//Tests an out of bounds syslog
//Should return -1
int main() {
    char *p = (char *) vm_extend();
    p[0] = 'H';
    vm_syslog(p + 1, 8192); //starting mid page and crossing the arena boundary
}