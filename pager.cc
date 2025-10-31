#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include "vm_pager.h"
#include <queue>
#include <cstring>

using namespace std;

//failing extend, read s
//virtual page struct
struct vpage_t {
    int disk_block;
    int ppage;
    bool valid;
    bool referenced;
    bool dirty;
    bool isZeroed;
};

//processs struct
struct process_t {
    pid_t pid;
    page_table_t *page_table;
    vector<vpage_t> vpages; 
};

// ASK JEANNIE TOMORROW AT OFFICE HOURS
//struct for the clock algorithm -- need to check
struct clock_entry_t {
    pid_t pid;
    unsigned int vpn; //specific virtual page to evict -- need to find out how to calc. ASK JEANNIE
};

//GLOBAL structures
unordered_map<pid_t, process_t*> process_table; //map for id to process
process_t *current_process = nullptr; //tracks current process
queue<unsigned int> free_phys_pages; //keeps track of the free physical pages
queue<unsigned int> free_disk_blocks; //keeps track of the free disk blocks
queue<clock_entry_t> clock_queue; //queue for the clock algorithm

//to keep track of total sizes we make in init
unsigned int NUM_PHYS_PAGE; 
unsigned int NUM_DISK_BLOCKS;

//other constants
unsigned int NUM_VPAGES = VM_ARENA_SIZE / VM_PAGESIZE;


/*
 * vm_init
 *
 * Called when the pager starts.  It should set up any internal data structures
 * needed by the pager, e.g. physical page bookkeeping, process table, disk
 * usage table, etc.
 *
 * vm_init is passed both the number of physical memory pages and the number
 * of disk blocks in the raw disk.
 */
void vm_init(unsigned int memory_pages, unsigned int disk_blocks) {
    /*
    - acts as the main() function
    - called once
    - memory_pages -> how many simulated physical pages you can use
    - disk_blocks -> how many simulated disk blocks you can swap to
    - push every frame number into a free physical page queue
    - do the same for disk_blocks 
    - make the clock queue
    - set page table base register to nullptr since we have not switched into a process yet
    */

    NUM_PHYS_PAGE = memory_pages;
    NUM_DISK_BLOCKS = disk_blocks;

    //empty physical page queue just in case
    while(!free_phys_pages.empty()) {
        free_phys_pages.pop();
    }

    //fill free physical page queue
    for(unsigned int i = 0; i < NUM_PHYS_PAGE; i++) {
        free_phys_pages.push(i);
    }

    //empty disk block queue just in case
    while(!free_disk_blocks.empty()) {
        free_disk_blocks.pop();
    }

    //fill disk blocks 
    for(unsigned int i = 0; i < NUM_DISK_BLOCKS; i++) {
        free_disk_blocks.push(i);
    }

    //initialize clock queue
    while(!clock_queue.empty()) {
        clock_queue.pop();
    }
    
    //clear and initalize process table and current process
    process_table.clear();
    current_process = nullptr;
    page_table_base_register = nullptr;
}


/*
 * vm_create
 *
 * Called when a new process, with process identifier "pid", is added to the
 * system.  It should create whatever new elements are required for each of
 * your data structures.  The new process will only run when it's switched
 * to via vm_switch().
 */
void vm_create(pid_t pid) {
    // create a new process struct
        // allocate memeory for a page table?
        // allocate memory for page entries?
    // add pid to map

    //allocate new process struct
    process_t *new_process = new process_t;
    new_process->pid = pid;
    new_process->page_table = new page_table_t;

    //extra
    new_process->vpages.clear();
    new_process->vpages.reserve(NUM_VPAGES);

    //set read and write bits to 0 for each pte
    // set the virtual page entry fields so page is invalid
    // ASK JEANNIE: DO WE NEED TO DO ALL OF THIS IN CREATE OR SHOULD WE DEFER THIS TO EXTEND
    for (unsigned int i = 0; i < NUM_VPAGES ; i++) {
        new_process->page_table->ptes[i].read_enable = 0;
        new_process->page_table->ptes[i].write_enable = 0;

        // new_process->vpages[i].disk_block = -1; //-1 because not in disk yet 
        // new_process->vpages[i].ppage = -1; //-1 because not in physical mem yet
        // new_process->vpages[i].valid = false;
        // new_process->vpages[i].referenced = false;
        // new_process->vpages[i].dirty = false;
        // new_process->vpages[i].isZeroed = true;
    }

    //add to map 
    process_table[pid] = new_process;  
}

/*
 * vm_extend
 *
 * A request by current process to declare as valid the lowest invalid virtual
 * page in the arena.  It should return the lowest-numbered byte of the new
 * valid virtual page.  E.g., if the valid part of the arena before calling
 * vm_extend is 0x60000000-0x60003fff, the return value will be 0x60004000,
 * and the resulting valid part of the arena will be 0x60000000-0x60005fff.
 * vm_extend should return NULL on error, e.g., if the disk is out of swap
 * space.
 */
void * vm_extend() {
    /*
    - go through current process virtual page array and find the first virtual page where valid = false
    - check to see if theres a free block, if the queue is empty return null
    - mark page as valid and assign a disk block (pop from the queue)
    - return the virtual address (HOW DO YOU CALCULATE THAT)
    */

    //check to see if there is space to allocate a disk block 
    if(free_disk_blocks.empty()) {
        return NULL;
    }

    //check to see if the arena is full
    if(current_process->vpages.size() >= NUM_VPAGES) {
        return NULL; 
    }

    //create a new virtual page and do the assignments and mark as valid 
    vpage_t new_page;
    new_page.disk_block = free_disk_blocks.front();
    free_disk_blocks.pop();

    //set all the values in the new vpage
    new_page.ppage = -1;
    new_page.valid = true;
    new_page.referenced = false;
    new_page.dirty = false;
    new_page.isZeroed = true;
    
    //add new page to current proces
    current_process->vpages.push_back(new_page);

    // setting read and write to 0 just in case so we know this will trigger a fault
    // page_table_entry_t &pte = current_process->page_table->ptes[i];
    // pte.read_enable = 0;
    // pte.write_enable = 0;

    void* addr = (void*)((uintptr_t) VM_ARENA_BASEADDR + ((current_process->vpages.size() -1) * VM_PAGESIZE));
    return addr;
}

/*
 * vm_switch
 *
 * Called when the kernel is switching to a new process, with process
 * identifier "pid".  This allows the pager to do any bookkeeping needed to
 * register the new process.
 */
void vm_switch(pid_t pid) {
    current_process = process_table[pid];
    page_table_base_register = current_process->page_table;
}

/*
 * vm_destroy
 *
 * Called when current process exits.  It should deallocate all resources
 * held by the current process (page table, physical pages, disk blocks, etc.)
 */
void vm_destroy() {
    // return 0;
}

//runs clock algorithm 
int clock_algo() {
    clock_entry_t tempClock = clock_queue.front();
    clock_queue.pop();
    
    //have the correct process now 
    process_t *victim = process_table[tempClock.pid];
    unsigned int vpn = tempClock.vpn;

    while(victim->vpages[vpn].referenced) {
        victim->vpages[vpn].referenced = false;
        victim->page_table->ptes[vpn].read_enable = 0;
        victim->page_table->ptes[vpn].write_enable = 0;

        //push back to the end of the queue 
        clock_queue.push(tempClock);

        //move clock hand
        tempClock = clock_queue.front();
        clock_queue.pop();
        victim = process_table[tempClock.pid];
        vpn = tempClock.vpn;
    }
    
    //now found page where referenced == 0
   
    //if dirty is 1 write to disk
    if(victim->vpages[vpn].dirty == 1) {
        disk_write(victim->vpages[vpn].disk_block, victim->vpages[vpn].ppage);
        victim->vpages[vpn].dirty = false;
    }
    int freed_page = victim->vpages[vpn].ppage;
    victim->vpages[vpn].ppage = -1;
    victim->page_table->ptes[vpn].read_enable = 0;
    victim->page_table->ptes[vpn].write_enable = 0;
    return freed_page;
}


// //helper to see if vpn has a valid mapping to a physical page
int inPhysicalMem(vpage_t &current_page, unsigned int vpn, bool write_flag) {
    if(current_page.ppage != -1) {
        current_process->page_table->ptes[vpn].read_enable = 1;
        current_page.referenced = true; //since resident and attempted to be accessed, it is referenced
        //later with clock, if its write flag or if its dirty because when second eviction dirty bit remains
        if(write_flag) {
            current_process->page_table->ptes[vpn].write_enable = 1;
            current_page.dirty = true; //write fault, so need to set dirty to 1
        }
        return 1; //sucess, just needed to reset the bits
    }
    return 0; //failure, problem was not about resetting the r/w bits
}

/*
 * vm_fault
 *
 * Called when current process has a fault at virtual address addr.  write_flag
 * is true if the access that caused the fault is a write.
 * Should return 0 on success, -1 on failure.
 */

// Couple conditions: Extend, Read; Extend, Write; 
// Page needs to be Zeroed on a Read

int vm_fault(void *addr, bool write_flag) {
    
    // calculate the current virtual page number we are on from the addr passed into function
    uintptr_t addr_int = (uintptr_t) addr;
    unsigned int vpn = (addr_int - (uintptr_t)VM_ARENA_BASEADDR) / VM_PAGESIZE;

    //TO DO: CHECK IF PAGE IS IN ARENA
    
    // check if current page is valid, if not something went wrong
    if (!current_process->vpages[vpn].valid) {
        return -1; // invalid access — not extended yet
    }

    //checks to see if page is resident, if so update permission 
    vpage_t &current_page = current_process->vpages[vpn];
    if(inPhysicalMem(current_page, vpn, write_flag)) {
        return 0;
    }

    // if physical space is free just allocate from queue and do the necessary assignments
    int ppage; 
    if(!free_phys_pages.empty()) {
       ppage = free_phys_pages.front();
       free_phys_pages.pop();
    }
    else {
        ppage = clock_algo();
    }

    //bring data into memory -- check zero logic 

    //should I fill this page with zeros again? Do either with first fault or either when you read to it
    if(current_page.isZeroed) {
        memset((char *)pm_physmem + (ppage * VM_PAGESIZE), 0, VM_PAGESIZE);
    }
    else {
        disk_read(current_page.disk_block, ppage);
    }

    //update metadata and page table
    current_page.ppage = ppage;
    current_page.referenced = true;
    current_page.dirty = write_flag;

    current_process->page_table->ptes[vpn].ppage = ppage;
    current_process->page_table->ptes[vpn].read_enable = 1;
    if(write_flag) {
        current_process->page_table->ptes[vpn].write_enable = 1;
        current_page.isZeroed = false;
    }
    // else {
    //     current_process->page_table->ptes[vpn].write_enable = 0;
    // }
    // ^ if you read to a page you already written to does not mean you need to reset write bit 

    //add to the clock queue
    clock_entry_t new_entry = { current_process->pid, vpn};
    clock_queue.push(new_entry);
    return 0; 
}

/*
 * vm_syslog
 *
 * A request by current process to log a message that is stored in the process'
 * arena at address "message" and is of length "len".
 *
 * Should return 0 on success, -1 on failure.
 */
int vm_syslog(void *message, unsigned int len) {
    return 0;
}


// int notPmm_freeSpace(struct vpage *current_page, unsigned int vpn) {
//     current_page->ppage = free_phys_pages.front();
//     free_phys_pages.pop();

//     disk_read(current_page->disk_block, current_page->ppage);

//     current_page->referenced = 1;
//     current_page->valid = true;

//     //TO DO: ASK IF YOU NEED TO SET THE R/W PERMISSIONS 
//     current_process->pte[vpn].read_enable = 1;

//     //if attempted to write to the page
//     if(write_flag) {
//         current_process->pte[vpn].write_enable = 1;
//         current_page->dirty = 1;
//     }
//     return 1;
// }

