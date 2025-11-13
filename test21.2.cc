#include "vm_app.h"
#include <iostream>

using namespace std;

/*
Tests eviction between clean and dirty pages
*/
int main() {
    char* clean = (char*) vm_extend(); 
    char* dirty = (char*) vm_extend();
    dirty[0] = 'Z';

    //keep extending to force an eviction
    for (int i = 0; i < 1000; i++) {
        vm_extend();
    }

    //Bring evicted page back
    if (dirty[0] != 'Z') {
        cout << "Dirty page not written to disk" << endl;
    }

    //Clean page brought back must be zeroed
    if (clean[0] != '\0') {
        cout << "clean page error" << endl;
    }

    return 0;
}
