/*
 * Lab Experiment 3: First-Come, First-Served (FCFS) CPU Scheduling
 * 
 * Objective: Implement FCFS CPU scheduling algorithm.
 * Input: Number of processes and burst times.
 * Output: Waiting time and turnaround time for each process,
 *         and calculate average waiting time and average turnaround time.
 */

#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
};

int main() {
    int n;

    printf("==================================================\n");
    printf(" Experiment 3: First-Come, First-Served (FCFS) CPU Scheduling\n");
    printf("==================================================\n\n");

    printf("Enter number of processes: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    struct Process *p = (struct Process *)malloc(n * sizeof(struct Process));
    if (p == NULL) {
        perror("Memory allocation failed");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("Enter burst time for Process P%d: ", p[i].pid);
        if (scanf("%d", &p[i].burst_time) != 1 || p[i].burst_time < 0) {
            printf("Invalid burst time entered.\n");
            free(p);
            return 1;
        }
    }

    // Calculating Waiting Time and Turnaround Time
    // Assuming arrival time = 0 for all processes in standard FCFS
    p[0].waiting_time = 0;
    p[0].completion_time = p[0].burst_time;
    p[0].turnaround_time = p[0].burst_time;

    for (int i = 1; i < n; i++) {
        p[i].waiting_time = p[i - 1].waiting_time + p[i - 1].burst_time;
        p[i].completion_time = p[i].waiting_time + p[i].burst_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
    }

    double total_wt = 0;
    double total_tat = 0;

    for (int i = 0; i < n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    printf("\n---------------------------------------------------------------------------\n");
    printf("| Process | Burst Time (BT) | Waiting Time (WT) | Turnaround Time (TAT) |\n");
    printf("---------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("|   P%-4d |      %-10d |        %-10d |          %-11d |\n",
               p[i].pid, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time);
    }
    printf("---------------------------------------------------------------------------\n\n");

    // Display Gantt Chart
    printf("Gantt Chart:\n");
    printf("+");
    for (int i = 0; i < n; i++) printf("-------+");
    printf("\n|");
    for (int i = 0; i < n; i++) printf("  P%-3d |", p[i].pid);
    printf("\n+");
    for (int i = 0; i < n; i++) printf("-------+");
    printf("\n0");
    for (int i = 0; i < n; i++) {
        printf("      %-2d", p[i].completion_time);
    }
    printf("\n\n");

    printf("Total Waiting Time     = %.2f\n", total_wt);
    printf("Average Waiting Time   = %.2f\n", total_wt / n);
    printf("Total Turnaround Time  = %.2f\n", total_tat);
    printf("Average Turnaround Time = %.2f\n", total_tat / n);
    printf("==================================================\n");

    free(p);
    return 0;
}
