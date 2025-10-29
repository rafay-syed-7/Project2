#include <assert.h>
#include <iostream>
#include <unordered_map>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include "vm_pager.h"

using namespace std;

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
    vpage_t vpages[VM_ARENA_SIZE / VM_PAGESIZE]; 
};

//struct for the clock algorithm -- need to check
struct clock_entry_t {
    pid_t pid;
    unsigned int vpn; //specific virtual page to evict -- need to find out how to calc. ASK JEANNIE
}

//GLOBAL structures
unordered_map<pid_t, process_t*> process_table; //map for id to process
process_t *current_process = nullptr //tracks current process
queue<unsigned_int> free_phys_pages; //keeps track of the free physical pages
queue<unsigned_int> free_disk_blocks; //keeps track of the free disk blocks
queue<clock_entry_t> clock_queue; //queue for the clock algorithm

//to keep track of total sizes we make in init
unsigned int NUM_PHYS_PAGE; 
unsigned int NUM_DISK_BLOCKS;


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
        free_phys_page.pop();
    }

    //fill free physical page queue
    for(int i = 0; i < NUM_PHYS_PAGE; i++) {
        free_phys_pages.push(i);
    }

    //empty disk block queue just in case
    while(!free_disk_blocks.empty()) {
        free_disk_blocks.pop();
    }

    //fill disk blocks 
    for(int i = 0; i < NUM_DISK_BLOCKS; i++) {
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

    //set read and write bits to 0 for each pte
    // set the virtual page entry fields so page is invalid
    // ASK JEANNIE: DO WE NEED TO DO ALL OF THIS IN CREATE OR SHOULD WE DEFER THIS TO EXTEND
    for(int i = 0; i < (VM_ARENA_SIZE/VM_PAGESIZE); i++) {
        new_process->page_table->ptes[i].read_enable = 0;
        new_process->page_table->ptes[i].write_enable = 0;

        new_process->vpages[i].disk_block = -1; //-1 because not in disk yet 
        new_process->vpages[i].ppage = -1; //-1 because not in physical mem yet
        new_process->vpages[i].valid = false;
        new_process->vpages[i].referenced = false;
        new_process->vpages[i].dirty = false;
        new_process->vpages[i].isZeroed = true;
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

    
}