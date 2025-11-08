#include <iostream>
#include "vm_app.h"
using namespace std;

//Tests allocating disk space until full
//pager should return null when disk space is full and not crash
int main() {
    while (vm_extend() != nullptr) {
        // keep extending until NULL returned
    }
    vm_syslog((char *)"disk full handled", 18);
}
