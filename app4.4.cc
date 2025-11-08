#include <iostream>
#include "vm_app.h"

using namespace std;

//Tests that pages are not mapped until they are accessed 
int main() {
    for (int i = 0; i < 10; i++) {
        vm_extend(); //allocates 10 pages, but doesn't access them 
    }
    vm_syslog((char *)"extended 10 pages without touching", 34);
}