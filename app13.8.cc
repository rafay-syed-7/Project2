#include "vm_app.h"
#include <iostream>
#include <string>
#include <cstring>


using namespace std;

/*
vm_destroy stress test
Allocates 8 pages, writes to them, yields and exits
*/

int main() {
    const int NUM_PAGES = 8;
    char* pages[NUM_PAGES];

    //allocate 8 pages
    for(int i = 0; i < NUM_PAGES; i++) {
        pages[i] = (char*) vm_extend();

        if(pages[i] == nullptr) {
            cout << "vm_extend() failed at page " << i << endl;
            return 1; //might be leak form earlier test
        }

        //write something unique to each page
        string message = "Page number " + to_string(i);

        //write data into virtual memory
        strcpy(pages[i], message.c_str());

        //verify syslog works 
        vm_syslog(pages[i], message.length());
    }

    for(int i = 0; i < 3; i++) {
        vm_yield();
    }

    cout << "returning so vm_destroy called" << endl;
    return 0;
}