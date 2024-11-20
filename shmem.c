#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "syscall.h"

#include "defs.h"
#include "memlayout.h"

int sys_shm_open(void) {
    int size;
    if (argint(0, &size) < 0)
        return -1;
    
    void *shmem = kalloc();  // Allocates memory for shared memory
    if (!shmem)
        return -1;

    // Additional shared memory initialization if necessary
    return (int)shmem;  // Return memory address as identifier
}

int sys_shm_attach(void) {
    int shmid;
    if (argint(0, &shmid) < 0)
        return -1;

    // Attach shared memory segment here
    return shmid;
}

