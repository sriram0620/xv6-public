// sharedfuncs.h
#ifndef SHAREDFUNCS_H
#define SHAREDFUNCS_H

int spawn(char *name, int priority);
void* shm_open(int size);
char *shm_attach(int shmid);
int thread_create(void (*fn)(void *), void *arg);
int thread_join(int tid);
// void mutex_lock(struct mutex *m);
// void mutex_unlock(struct mutex *m);
int signal(int pid, int signum);

#endif
