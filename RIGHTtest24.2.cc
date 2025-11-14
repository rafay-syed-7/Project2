#include <iostream>
#include "vm_app.h"

using namespace std;

// Tries to catch library M (Vm extend asserts if out of disk space)

int main() {
    for (int i = 0; i < 200000; i++) {
        char *p = (char *) vm_extend();
        p[0] = 'A' + i;
    }
}