/*
 * Lab Experiment 5: Inter-Process Communication using Anonymous Pipe & dup2()
 * 
 * Objective: Create parent and child processes using fork().
 * Use pipe() and dup2() to redirect:
 *   - Parent's standard output (stdout) to pipe write-end
 *   - Child's standard input (stdin) from pipe read-end
 * Transfer a message from parent to child and display it in child.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 512

int main() {
    int pipefd[2]; // pipefd[0] = read end, pipefd[1] = write end
    pid_t pid;

    printf("==================================================\n");
    printf(" Experiment 5: IPC using Anonymous Pipe and dup2()\n");
    printf("==================================================\n\n");

    // Create an anonymous pipe
    if (pipe(pipefd) == -1) {
        perror("pipe() failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // --- CHILD PROCESS ---
        // Child reads from the pipe redirected as STDIN
        close(pipefd[1]); // Close unused write end

        // Redirect stdin (fd 0) to pipefd[0]
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2() child failed");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]); // Close original read end after duplicating

        char received_msg[BUFFER_SIZE];
        // Read directly from standard input (which is redirected from pipe)
        if (fgets(received_msg, sizeof(received_msg), stdin) != NULL) {
            // Write directly to stderr or stdout
            // Notice: child's stdout is still normal terminal stdout
            fprintf(stdout, "[Child Process] Successfully read message from redirected stdin:\n");
            fprintf(stdout, "                \"%s\"\n", received_msg);
        } else {
            fprintf(stderr, "[Child Process] Error or EOF reading from stdin\n");
        }

        exit(EXIT_SUCCESS);
    } else {
        // --- PARENT PROCESS ---
        // Parent writes to the pipe redirected as STDOUT
        close(pipefd[0]); // Close unused read end

        // Duplicate STDOUT to pipe write end
        int saved_stdout = dup(STDOUT_FILENO); // Save original stdout to restore later
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2() parent failed");
            exit(EXIT_FAILURE);
        }
        close(pipefd[1]); // Close original write end after duplicating

        // Any printf now writes into the pipe!
        printf("Hello from Parent process! IPC via pipe and dup2() succeeded.\n");
        fflush(stdout); // Ensure output is flushed into the pipe

        // Restore parent's stdout
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);

        printf("[Parent Process] Message sent to child via redirected stdout.\n");
        printf("[Parent Process] Waiting for child process to finish...\n");

        wait(NULL); // Wait for child process to finish
        printf("[Parent Process] Child finished. IPC demonstration complete.\n");
        printf("==================================================\n");
    }

    return 0;
}
