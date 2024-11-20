#ifndef SPAWN_H
#define SPAWN_H

#include "types.h"
#include "user.h"
#include "fcntl.h"

int spawn(char *filename, int priority) { // Make this match user.h
    // Implementation of spawn function
    // Use `priority` as needed within the function
    int pid = fork();
    if (pid == 0) {
        // Child process: execute the file
        exec(filename, (char **)0);
        exit();
    } else if (pid > 0) {
        // Parent process: return child's PID
        return pid;
    } else {
        // Error occurred
        return -1;
    }
}
#endif