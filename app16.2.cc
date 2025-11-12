#include "vm_app.h"
#include <iostream>
#include <cstring>

using namespace std;

//tests successive writes on the same virtual page
//verifying dirty bit and write permission  

int main() {
    char *page = (char *) vm_extend();

    //first write
    strcpy(page, "Hello, hello!");

    //second write, overwriting part of the same page
    strcpy(page + 7, "pager.");

    //third write, overwrite the beginnign
    strcpy(page, "OS!");

    vm_syslog(page, 12);

    return 0;
}