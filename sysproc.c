#include "sharedfuncs.h"
#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

// Function prototypes for system calls
int sys_fork(void);
int sys_exit(void);
int sys_wait(void);
int sys_pipe(void);
int sys_read(void);
int sys_kill(void);
int sys_exec(void);
int sys_fstat(void);
int sys_chdir(void);
int sys_dup(void);
int sys_getpid(void);
int sys_sbrk(void);
int sys_sleep(void);
int sys_uptime(void);
int sys_open(void);
int sys_write(void);
int sys_mknod(void);
int sys_unlink(void);
int sys_link(void);
int sys_mkdir(void);
int sys_close(void);

// Project-specific function prototypes

// System call implementations
int sys_fork(void) {
  return fork();
}

int sys_exit(void) {
  exit();
  return 0;  // not reached
}

int sys_wait(void) {
  return wait();
}

int sys_kill(void) {
  int pid;
  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void) {
  return myproc()->pid;
}

int sys_sbrk(void) {
  int addr;
  int n;
  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void) {
  int n;
  uint ticks0;
  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

int sys_uptime(void) {
  uint xticks;
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Project-specific system calls

int sys_spawn(void) {
    char *name;
    int priority;

    // Get arguments from user space
    if (argstr(0, &name) < 0 || argint(1, &priority) < 0) {
        return -1;
    }

    // Call your spawn function, which could be fork or a custom function.
    // Here it’s assumed spawn creates a new process or thread
    return spawn(name, priority);  // `spawn` is your own function, assuming you already have it
}

// Function to create a new thread
int sys_thread_create(void) {
    void (*start_routine)(void*);
    void *arg;

    // Get arguments from user space
    if (argint(0, (int*)&start_routine) < 0 || argint(1, (int*)&arg) < 0) {
        return -1;
    }

    // Create a new thread
    return thread_create(start_routine, arg);  // `thread_create` is your custom thread creation function
}

int sys_thread_join(void) {
    int tid;
    
    // Get the thread ID from the user
    if (argint(0, &tid) < 0) {
        return -1;
    }

    // Wait for the thread to finish
    return thread_join(tid);  // `thread_join` is your custom function to join threads
}

// int sys_mutex_lock(void) {
//     struct mutex *m;

//     // Get the mutex pointer from user space
//     if (argint(0, (int*)&m) < 0) {
//         return -1;
//     }

//     // Lock the mutex
//     return mutex_lock(m);  // `mutex_lock` is your custom function to lock the mutex
// }

// int sys_mutex_unlock(void) {
//     struct mutex *m;

//     // Get the mutex pointer from user space
//     if (argint(0, (int*)&m) < 0) {
//         return -1;
//     }

//     // Unlock the mutex
//     return mutex_unlock(m);  // `mutex_unlock` is your custom function to unlock the mutex
// }
