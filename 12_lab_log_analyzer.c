/*
 * Lab Experiment 12: Concurrent Log File Analysis using fork() and POSIX Threads
 * 
 * Objective:
 *   1. Analyze a log file containing INFO, WARNING, and ERROR messages.
 *   2. Use fork() to divide workload between parent and child process.
 *      - Parent analyzes the first half of the log file.
 *      - Child analyzes the second half of the log file.
 *   3. Within each process:
 *      - Create Thread 1 using pthread_create() to count INFO messages.
 *      - Create Thread 2 using pthread_create() to count ERROR messages.
 *      - Main thread flow counts WARNING messages.
 *   4. Aggregate counts using an IPC pipe from child to parent and display totals.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINES 1000
#define LINE_LEN 256

// Shared lines buffer loaded in memory for the process chunk
char lines_chunk[MAX_LINES][LINE_LEN];
int chunk_line_count = 0;

// Thread arguments and results
typedef struct {
    const char *keyword;
    int count;
} ThreadFilter;

void *count_occurrences(void *arg) {
    ThreadFilter *filter = (ThreadFilter *)arg;
    filter->count = 0;

    for (int i = 0; i < chunk_line_count; i++) {
        if (strstr(lines_chunk[i], filter->keyword) != NULL) {
            filter->count++;
        }
    }

    pthread_exit(NULL);
}

// Helper function to create sample log file if not already present
void create_sample_log_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to create log file");
        exit(EXIT_FAILURE);
    }
    // Writing sample lines (Part 1 - for Parent)
    fprintf(fp, "[2026-10-11 10:00:01] INFO System boot initialized\n");
    fprintf(fp, "[2026-10-11 10:00:02] WARNING Disk usage above 75 percent\n");
    fprintf(fp, "[2026-10-11 10:00:03] INFO User login successful: user_id=42\n");
    fprintf(fp, "[2026-10-11 10:00:04] ERROR Database connection timeout\n");
    fprintf(fp, "[2026-10-11 10:00:05] WARNING CPU temperature above threshold\n");
    fprintf(fp, "[2026-10-11 10:00:06] INFO Service daemon started\n");
    
    // Writing sample lines (Part 2 - for Child)
    fprintf(fp, "[2026-10-11 10:00:07] INFO Periodic backup scheduled\n");
    fprintf(fp, "[2026-10-11 10:00:08] ERROR Null pointer exception in auth service\n");
    fprintf(fp, "[2026-10-11 10:00:09] WARNING High memory allocation rate\n");
    fprintf(fp, "[2026-10-11 10:00:10] ERROR Port 8080 already in use\n");
    fprintf(fp, "[2026-10-11 10:00:11] INFO Data synchronization complete\n");
    fprintf(fp, "[2026-10-11 10:00:12] WARNING Network latency spike detected\n");
    fclose(fp);
}

int main() {
    const char *log_filename = "system_log.txt";
    create_sample_log_file(log_filename);

    printf("=================================================================\n");
    printf(" Experiment 12: Concurrent Log File Analysis (fork + pthreads)\n");
    printf("=================================================================\n\n");

    // Read entire log file into an array of lines
    FILE *fp = fopen(log_filename, "r");
    if (!fp) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }

    char all_lines[MAX_LINES][LINE_LEN];
    int total_lines = 0;
    while (fgets(all_lines[total_lines], LINE_LEN, fp) && total_lines < MAX_LINES) {
        total_lines++;
    }
    fclose(fp);

    printf("Total log lines in file '%s': %d lines\n\n", log_filename, total_lines);

    int mid = total_lines / 2;

    // Create an IPC pipe so Child can transmit its counts back to Parent
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe() failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // ====================== CHILD PROCESS ======================
        // Processes the second half: from index mid to total_lines
        close(pipefd[0]); // Close pipe read end

        chunk_line_count = 0;
        for (int i = mid; i < total_lines; i++) {
            strncpy(lines_chunk[chunk_line_count], all_lines[i], LINE_LEN);
            chunk_line_count++;
        }

        pthread_t t_info, t_error;
        ThreadFilter info_filter = {"INFO", 0};
        ThreadFilter error_filter = {"ERROR", 0};

        // Create threads for INFO and ERROR
        pthread_create(&t_info, NULL, count_occurrences, &info_filter);
        pthread_create(&t_error, NULL, count_occurrences, &error_filter);

        // Main thread flow counts WARNING
        int warning_count = 0;
        for (int i = 0; i < chunk_line_count; i++) {
            if (strstr(lines_chunk[i], "WARNING") != NULL) {
                warning_count++;
            }
        }

        // Wait for worker threads to finish
        pthread_join(t_info, NULL);
        pthread_join(t_error, NULL);

        printf("[Child PID %d] Processed lines %d to %d (Count = %d lines)\n",
               getpid(), mid + 1, total_lines, chunk_line_count);
        printf("  -> Child INFO count:    %d\n", info_filter.count);
        printf("  -> Child ERROR count:   %d\n", error_filter.count);
        printf("  -> Child WARNING count: %d\n\n", warning_count);

        // Send results array [info, warning, error] to parent via pipe
        int child_results[3] = {info_filter.count, warning_count, error_filter.count};
        write(pipefd[1], child_results, sizeof(child_results));
        close(pipefd[1]);

        exit(EXIT_SUCCESS);
    } else {
        // ====================== PARENT PROCESS ======================
        // Processes the first half: from index 0 to mid
        close(pipefd[1]); // Close pipe write end

        chunk_line_count = 0;
        for (int i = 0; i < mid; i++) {
            strncpy(lines_chunk[chunk_line_count], all_lines[i], LINE_LEN);
            chunk_line_count++;
        }

        pthread_t t_info, t_error;
        ThreadFilter info_filter = {"INFO", 0};
        ThreadFilter error_filter = {"ERROR", 0};

        // Create threads for INFO and ERROR
        pthread_create(&t_info, NULL, count_occurrences, &info_filter);
        pthread_create(&t_error, NULL, count_occurrences, &error_filter);

        // Main thread flow counts WARNING
        int warning_count = 0;
        for (int i = 0; i < chunk_line_count; i++) {
            if (strstr(lines_chunk[i], "WARNING") != NULL) {
                warning_count++;
            }
        }

        // Wait for worker threads
        pthread_join(t_info, NULL);
        pthread_join(t_error, NULL);

        printf("[Parent PID %d] Processed lines 1 to %d (Count = %d lines)\n",
               getpid(), mid, chunk_line_count);
        printf("  -> Parent INFO count:    %d\n", info_filter.count);
        printf("  -> Parent ERROR count:   %d\n", error_filter.count);
        printf("  -> Parent WARNING count: %d\n\n", warning_count);

        // Read child counts from pipe
        int child_results[3];
        read(pipefd[0], child_results, sizeof(child_results));
        close(pipefd[0]);

        wait(NULL); // Wait for child to exit

        int total_info = info_filter.count + child_results[0];
        int total_warning = warning_count + child_results[1];
        int total_error = error_filter.count + child_results[2];

        printf("-----------------------------------------------------------------\n");
        printf(" FINAL CONSOLIDATED LOG ANALYSIS REPORT:\n");
        printf("-----------------------------------------------------------------\n");
        printf(" Total INFO messages:    %d (Parent: %d, Child: %d)\n",
               total_info, info_filter.count, child_results[0]);
        printf(" Total WARNING messages: %d (Parent: %d, Child: %d)\n",
               total_warning, warning_count, child_results[1]);
        printf(" Total ERROR messages:   %d (Parent: %d, Child: %d)\n",
               total_error, error_filter.count, child_results[2]);
        printf(" Total analyzed entries: %d\n", total_info + total_warning + total_error);
        printf("=================================================================\n");
    }

    return 0;
}
