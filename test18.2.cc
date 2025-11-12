#include <iostream>
#include "vm_app.h"
using namespace std;

// Destroy stress test


void destroy_stress_test() {
    for (int i = 0; i < 2000; i++) {
        char *p = (char *) vm_extend();
        p[0] = 'A' + i;
    }
    
    return; // triggers vm_destroy()

}


int main() {
    for (int i = 0; i < 100; i++) {
        destroy_stress_test();
    }

}