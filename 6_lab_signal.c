/*
 * Lab Experiment 6: Signal Handling (SIGINT and SIGCHLD)
 * 
 * Objective:
 *   1. Register signal handler for SIGINT (Ctrl+C) and display an appropriate message.
 *   2. Implement a parent-child program using SIGCHLD to asynchronously detect
 *      child termination and perform the required wait() operation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

volatile sig_atomic_t sigint_received = 0;
volatile sig_atomic_t child_terminated = 0;

// Handler for SIGINT
void handle_sigint(int sig) {
    (void)sig; // suppress unused parameter warning
    // Signal handlers should be safe and concise
    const char msg[] = "\n[SIGNAL HANDLER] SIGINT (Signal 2) caught! Interruption handled gracefully.\n";
    write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    sigint_received = 1;
}

// Handler for SIGCHLD
void handle_sigchld(int sig) {
    (void)sig;
    int saved_errno = errno;
    int status;
    pid_t pid;

    // Reap all terminated zombie children
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        char buf[128];
        int len;
        if (WIFEXITED(status)) {
            len = snprintf(buf, sizeof(buf),
                "[SIGNAL HANDLER: SIGCHLD] Child with PID %d terminated with exit status %d.\n",
                pid, WEXITSTATUS(status));
        } else {
            len = snprintf(buf, sizeof(buf),
                "[SIGNAL HANDLER: SIGCHLD] Child with PID %d terminated abnormally.\n", pid);
        }
        if (len > 0) {
            write(STDOUT_FILENO, buf, len);
        }
        child_terminated = 1;
    }
    errno = saved_errno;
}

int main() {
    printf("==================================================\n");
    printf(" Experiment 6: Signal Handling (SIGINT & SIGCHLD)\n");
    printf("==================================================\n\n");

    // 1. Setting up SIGINT handler using sigaction
    struct sigaction sa_int;
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("Failed to register SIGINT handler");
        exit(EXIT_FAILURE);
    }
    printf("[Setup] SIGINT handler registered. (Try pressing Ctrl+C or kill -SIGINT %d)\n", getpid());

    // 2. Setting up SIGCHLD handler using sigaction
    struct sigaction sa_chld;
    sa_chld.sa_handler = handle_sigchld;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa_chld, NULL) == -1) {
        perror("Failed to register SIGCHLD handler");
        exit(EXIT_FAILURE);
    }
    printf("[Setup] SIGCHLD handler registered.\n\n");

    // Demonstrating SIGINT delivery via raise()
    printf("Sending SIGINT to current process via raise(SIGINT)...\n");
    raise(SIGINT);

    printf("\nCreating child process to demonstrate SIGCHLD detection...\n");
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child
        printf("  [Child: PID %d] Running and doing work for 2 seconds...\n", getpid());
        sleep(2);
        printf("  [Child: PID %d] Finished work, terminating now with status 10.\n", getpid());
        exit(10);
    } else {
        // Parent does not block on wait() immediately; continues doing its work
        printf("[Parent: PID %d] Child spawned (PID %d). Parent continuing execution...\n", getpid(), pid);
        
        while (!child_terminated) {
            printf("[Parent] Waiting for SIGCHLD event... (sleeping 1s)\n");
            sleep(1);
        }

        printf("\n[Parent] Child termination successfully handled via SIGCHLD!\n");
        printf("==================================================\n");
    }

    return 0;
}
