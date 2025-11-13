#include "vm_app.h"
#include <iostream>
using namespace std;

/*
Tests disk read and disk write
A page written to the disk isnt restored correctly when we page back in 
*/
int main() {
    char* p = (char*) vm_extend();
    p[0] = 'A';
    p[1] = 'B';
    p[2] = 'C';

    //Force eviction
    for (int i = 0; i < 50; i++) {
        vm_extend(); 
    }

    //access p again, this should come from the disk
    char val0 = p[0];
    char val1 = p[1];
    char val2 = p[2];

    return 0;
}