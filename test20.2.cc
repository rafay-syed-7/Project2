#include "vm_app.h"
#include <iostream>

using namespace std;

/*
Tests that dirty bit is set when we write 
*/
int main() {
    char* p = (char*) vm_extend();
    p[0] = 'A'; //write attempt

    vm_yield();

    // Touch again, shouldn't reload from disk
    if (p[0] != 'X') {
        cout << "ERROR" << endl;
    }

    return 0;
}