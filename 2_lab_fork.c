/*
 * Lab Experiment 2: Process Life Cycle and Synchronization
 * 
 * Objective: Use fork(), getpid(), sleep(), and wait() to demonstrate
 * parent and child process creation, process states (NEW, READY, RUNNING,
 * WAITING, TERMINATED), and synchronization between parent and child.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int status;

    printf("==================================================\n");
    printf(" Experiment 2: Process Life Cycle & Synchronization\n");
    printf("==================================================\n\n");

    printf("[Parent] Main process starting. PID = %d\n", getpid());
    printf("[State: NEW] fork() is about to be invoked to create a child process...\n\n");

    pid = fork();

    if (pid < 0) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child Process
        printf("----------------- CHILD PROCESS -----------------\n");
        printf("[Child: READY -> RUNNING] Child is now executing!\n");
        printf("[Child] PID = %d, Parent PID = %d\n", getpid(), getppid());
        
        printf("[Child: RUNNING -> WAITING] Child going to sleep for 2 seconds (simulating I/O / blocking)...\n");
        sleep(2);
        
        printf("[Child: WAITING -> READY -> RUNNING] Child woke up from sleep.\n");
        printf("[Child: RUNNING -> TERMINATED] Child performing exit with exit code 42.\n");
        printf("-------------------------------------------------\n");
        exit(42);
    } else {
        // Parent Process
        printf("[Parent] Created child process with PID = %d\n", pid);
        printf("[Parent: WAITING] Parent synchronizing with child using wait()...\n");
        
        pid_t child_pid = wait(&status);
        
        printf("\n[Parent: RUNNING] wait() returned! Child (PID: %d) has terminated.\n", child_pid);
        
        if (WIFEXITED(status)) {
            printf("[Parent] Child process terminated normally with exit status: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("[Parent] Child process killed by signal: %d\n", WTERMSIG(status));
        }
        
        printf("[Parent] Parent completing execution and terminating.\n");
        printf("==================================================\n");
    }

    return 0;
}
