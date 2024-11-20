// shm_open.c
#include "types.h"
#include "user.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

// Define a static pointer for shared memory
static char *shm_ptr = 0;

void* shm_open(int size) {
    if (shm_ptr == 0) {
        shm_ptr = kalloc(); // Allocate one page for simplicity
        if (shm_ptr == 0) return 0; // Allocation failed
    }
    
    // Return the shared memory address
    return shm_ptr;
}
