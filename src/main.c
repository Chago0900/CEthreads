#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#define STACK_SIZE (1024 * 1024) // 1 MB stack size

// Function that the new thread will run
int func(void *arg) {
    printf("Inside func.\n");
    sleep(1);
    printf("Terminating func...\n");
    return 0;
}

int main() {
    printf("This process pid: %u\n", getpid());
    char status_file[] = "/proc/self/status";
    
    // Allocate stack for the new thread
    void *child_stack = malloc(STACK_SIZE);
    if (!child_stack) {
        perror("Failed to allocate stack");
        return 1;
    }

    pid_t thread_pid;

    printf("Creating new thread...\n");
    // Create the new thread with CLONE_THREAD
    thread_pid = clone(func, (char *)child_stack + STACK_SIZE, 
                   SIGCHLD | CLONE_SIGHAND | CLONE_FS | CLONE_VM | CLONE_FILES | CLONE_THREAD, 
                   NULL);

    
    if (thread_pid == -1) {
        perror("Failed to create thread");
        free(child_stack);
        return 1;
    }

    printf("Done! Thread pid: %d\n", thread_pid);

    // Wait for the child thread to finish
    waitpid(thread_pid, NULL, 0);

    // Optionally read from /proc/self/status
    FILE *fp = fopen(status_file, "rb");
    if (fp == NULL) {
        perror("Failed to open status file");
        free(child_stack);
        return 1;
    }

    printf("Looking into %s...\n", status_file);

    while (1) {
        char ch = fgetc(fp);
        if (feof(fp)) break;
        printf("%c", ch);
    }

    fclose(fp);
    free(child_stack); // Free the allocated stack

    return 0;
}
