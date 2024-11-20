#ifndef THREAD_JOIN_C
#define THREAD_JOIN_C






// thread_join.c
#include "types.h"
#include "user.h"
#include "proc.h"

// Wait for the specified thread to finish execution
int thread_join(int pid) {
    int status;
    if (waitpid(pid, &status, 0) < 0) {
        return -1; // Error if waiting failed
    }
    return status; // Return the exit status of the thread
}

// Your thread_join function implementation

#endif // THREAD_JOIN_C