#include <iostream>
#include "vm_app.h"
using namespace std;

//Tests read faults
int main() {
    char *p = (char *) vm_extend();
    p[0] = 'R';
    char x = p[0]; 
    vm_syslog(&x, 1);
}