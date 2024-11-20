#ifndef SPINLOCK_H
#define SPINLOCK_H

struct spinlock {
    uint locked;       // Is the lock held?

    // For debugging:
    char *name;        // Name of lock.
    struct cpu *cpu;   // The CPU holding the lock.
    uint pcs[10];      // Call stack (an array of program counters) that locked the lock.
};

void acquire(struct spinlock *lk);
void release(struct spinlock *lk);

#endif // SPINLOCK_H
