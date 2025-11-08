#include <iostream>
#include "vm_app.h"
using namespace std;

//run out of physical memory: use -m 2
int main() {
    vm_extend();
    vm_extend();
    vm_extend(); // should trigger eviction or fault
    vm_syslog((char *)"no phys mem left", 17);
}