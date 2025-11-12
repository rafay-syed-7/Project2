#include <iostream>
#include "vm_app.h"
using namespace std;

//Tests many r/w 
int main() {
    for (int i = 0; i < 20; i++) {
        char *p = (char *) vm_extend();
        p[0] = 'A' + (i % 26); //cycles through A....Z
        vm_syslog(p, 1);
    }
    vm_yield();
}