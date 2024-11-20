#ifndef THREAD_CREATE_C
#define THREAD_CREATE_C



// thread_create.c
#include "types.h"
#include "user.h"
#include "x86.h"
#include "proc.h"

// Simple thread creation function
int thread_create(void (*start_routine)(void*), void *arg) {
    int pid = fork();
    if (pid == 0) {
        // New thread, run the start routine
        start_routine(arg);
        exit();
    }
    return pid;
}
// Your thread_create function implementation

#endif // THREAD_CREATE_C