#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024) // 1 MB stack size

pid_t CEthread_create(int (*func)(void *), void *arg) {
    // Allocate stack for the new thread
    void *child_stack = malloc(STACK_SIZE);
    if (!child_stack) {
        perror("Failed to allocate stack");
        return -1;
    }

    // Create the new thread with CLONE_THREAD
    pid_t thread_pid = clone(func, (char *)child_stack + STACK_SIZE, 
                             SIGCHLD | CLONE_SIGHAND | CLONE_FS | CLONE_VM | CLONE_FILES | CLONE_THREAD, 
                             arg);

    if (thread_pid == -1) {
        perror("Failed to create thread");
        free(child_stack);
        return -1;
    }

    return thread_pid;
}

void CEthread_join(pid_t thread_pid) {
    // Wait for the child thread to finish
    waitpid(thread_pid, NULL, 0);
}
