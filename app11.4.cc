#include <iostream>
#include "vm_app.h"
using namespace std;

//Extends until disk is full

int main() {
    int count = 0;
    while (vm_extend() != nullptr) {
        count++;
    }
    vm_syslog((char *)"disk full stop", 14);
}