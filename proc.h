#ifndef PROC_H
#define PROC_H

#include "types.h"
#include "stat.h"
#include "user.h"
#include "x86.h"
#include "param.h"
#include "mmu.h"
#include "traps.h"
#include "spinlock.h"

// CPU state
struct cpu {
    uchar apicid;                // Local APIC ID
    struct context *scheduler;   // swtch() here to enter scheduler
    struct taskstate ts;         // Used by x86 to find stack for interrupt
    struct segdesc gdt[NSEGS];   // x86 global descriptor table
    volatile uint started;       // Has the CPU started?
    int ncli;                    // Depth of pushcli nesting
    int intena;                  // Were interrupts enabled before pushcli?
    struct proc *proc;           // Process running on this cpu or null
};

extern struct cpu cpus[NCPU];
extern int ncpu;
// extern struct proc proc[];    // Array of all processes

// Process state enum
enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };

// Process structure
struct proc {
    void *stack;                 // User-space stack for threads
    int tid;                     // Thread ID to identify threads separately
    int is_thread;               // Flag to indicate if the process is a thread
    uint sz;                     // Size of process memory (bytes)
    pde_t* pgdir;                // Page table
    char *kstack;                // Kernel stack for this process
    enum procstate state;        // Process state
    int pid;                     // Process ID
    struct proc *parent;         // Parent process
    struct trapframe *tf;        // Trap frame for current syscall
    struct context *context;     // swtch() here to run process
    void *chan;                  // Sleeping on chan if non-zero
    int killed;                  // Non-zero if killed
    struct file *ofile[NOFILE];  // Open files
    struct inode *cwd;           // Current directory
    char name[16];               // Process name (for debugging)
    uint pending_signals;        // Bitmask of pending signals
    int priority;                // Process priority
};

// Function to find a process by PID
struct proc* find_proc_by_pid(int pid);

#endif // PROC_H
