#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "user.h"

#ifndef MUTEX_H
#define MUTEX_H

// Functions for mutex handling
// void mutex_lock(struct mutex *m);
// void mutex_unlock(struct mutex *m);


// void mutex_lock(struct mutex *m) {
//     acquire(&m->lock);

//     // Wait until mutex is unlocked
//     while (m->locked) {
//         sleep(m, &m->lock);  // Wait for the mutex to be unlocked
//     }
//     m->locked = 1;

//     release(&m->lock);
// }

// void mutex_unlock(struct mutex *m) {
//     acquire(&m->lock);

//     m->locked = 0;
//     wakeup(m);  // Wake up any threads waiting on the mutex

//     release(&m->lock);
// }


#endif