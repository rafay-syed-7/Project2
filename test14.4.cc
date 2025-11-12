#include "vm_app.h"
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

/*
Page filled with zeroes
    - Written to disk after eviction
    - Read back correctly from disk, ie still has zeroes
    - Cleaned up the right way in destroy
*/

int main() {
    const int V_PAGES = 6;  
    char* pages[V_PAGES];

    //extend pages
    for(int i = 0; i < V_PAGES) {
        pages[i] = (char *) vm_extend();
    }

    //trigger read fault
    for(int i = 0; i < V_PAGES) {
        char val = pages[i][0]; //bring into memory
        (void) val; //stop compilier warnings
    }

    //yield to force evictions and disk writes
    for(int i = 0; i < 10; i++) {
        vm_yield();
    }

    //read to trigger page ins and check there still zero
    bool allZero = true;
    for (int i = 0; i < V_PAGES; i++) {
        if (pages[i][0] != '\0' || pages[i][100] != '\0') {
            cout << "ERROR - page " << i << endl;
            allZero = false;
        }
    }

    //should log no visible chars since its a zeroed page
    for (int i = 0; i < V_PAGES i++) {
        vm_syslog(pages[i], 5);
    }

    return 0;
}