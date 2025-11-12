#include "vm_app.h"
#include <iostream>
#include <cstring>

using namespace std;

//tests syslog edge cases

int main() {
    //allocate two contingious pages
    char *page1 = (char*) vm_extend();
    char *page2 = (char*) vm_extend();

    //fill with data
    strcpy(page1, "blah1");
    strcpy(page2, "beep2");

    //syslog across a page boundary - start near end of page and span into next -- tests cross page reads
    char *cross = page1 + VM_PAGESIZE - 3; //move pointer to 3 bytes before end of the page
    cross[0] = 'A';
    cross[1] = 'B';
    cross[2] = 'C'; //cross now fills the last 3 bytes

    page2[0] = 'D';
    page2[1] = 'E'; //writes to the start of page 2
    vm_syslog(cross, 5); //print 5 bytes starting at cross pointer

    //syslog on unmapped (outside the arena)
    char *invalid = page2 + VM_PAGESIZE + 1; //byte over 
    vm_syslog(invalid, 5); //should return error

    //zero length syslog
    vm_syslog(page1, 0);

    //syslog longer than a page
    vm_syslog(page1, VM_PAGESIZE + 10);
}