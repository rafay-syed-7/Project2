#include "vm_app.h"
#include <iostream>
#include <cstring>

using namespace std;

/*
Checks that clock algo works
- we have a hot page that should never be evicted because it is always referenced before the reference bit is cleared 

*/
int main() {
    const int NUM = 6;  
    char* pages[NUM];

    for (int i = 0; i < NUM; i++) {
        pages[i] = (char*) vm_extend();
        pages[i][0] = 'A' + i; //unique marking 
    }

    //page that is constantly referenced, so wont be kicked
    char* hot = pages[0];

    //force faults causing evictions
    for (int i = 0; i < 500; i++) {
        //make sure hot page is referenced repeatadely -- THIS PAGE SHOULD NEVER BE EVICTED
        char x = hot[0];  

        char* temp = (char*) vm_extend();
        if (temp != nullptr) {
            temp[0] = 'X';  //write fault
        }
    }

    //check if it got evicted
    // if (hot[0] != 'A') {
    //     cout << "BUG" << endl;
    // } else {
    //     cout << "NO BUG" << endl;
    // }

    return 0;
}
