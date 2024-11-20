#include "types.h"
#include "user.h"
#include <stddef.h> 
#include "stat.h"
// #include <stdlib.h>  // Standard library for exit()


#define SIGUSR1 1  // Example, adjust as per your implementation

// struct mutex *m;
void test_process_creation() {
    int pid = spawn("priority_task", 5);
    if (pid < 0) {
        printf(1, "Process creation failed\n");
    } else {
        printf(1, "Process created with PID %d\n", pid);
    }
}

void test_ipc() {
    void* shmid = shm_open(4096);
      if (shmid == NULL) {
        printf(1, "Shared memory allocation failed\n");
        return;
        }
    char *shmem = shm_attach((int)shmid);
        if (shmem == NULL) {
        printf(1, "Failed to attach to shared memory\n");
        return;
        }
    strcpy(shmem, "Message from Process A");
    printf(1, "Message in shared memory: %s\n", shmem);
}

void thread_func(void *arg) {
    printf(1, "Thread ID %d running\n", (int)arg);
}

void test_threads() {
    int tid = thread_create(thread_func, (void *)1);
    if (tid < 0) {
        printf(1, "Thread creation failed\n");
    } else {
        printf(1, "Thread created with TID %d\n", tid);
    }
    thread_join(tid);
}

// void test_mutex() {
//     // Acquire the lock using mutex_lock
//     printf(1, "Acquiring lock...\n");
//     mutex_lock(&m);  // No arguments

//     // Critical section
//     printf(1, "In critical section.\n");

//     // Release the lock
//     printf(1, "Releasing lock...\n");
//     mutex_unlock(&m);  // No arguments
// }


void test_signal() {
    int pid = getpid();  // Get the current process ID
    signal(pid, SIGUSR1);
    printf(1, "Signal sent to process %d\n", pid);
}


int main(int argc, char *argv[]) {
    printf(1, "Testing custom system calls...\n");

    test_process_creation();
    test_ipc();
    test_threads();
    // test_mutex();
    test_signal();

    return 0;
}
