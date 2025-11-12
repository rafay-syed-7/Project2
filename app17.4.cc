#include "vm_app.h"
#include <iostream>

using namespace std;

//does yield thousands of time -- stressing context switching
//makes sure theres no unnecessary faults

int main() {
    char *page1 = (char *) vm_extend();
    char *page2 = (char *) vm_extend();

    page1[0] = 'A';
    page2[0] = 'B';

    //yield 1000 times
    for (int i = 0; i < 100000; i++) {
        vm_yield();
    }

    // After many yields, pages should still hold original values
    if (page1[0] != 'A' || page2[0] != 'B') {
        cout << "didnt work" << endl;
        return 1;
    }

    return 0
}