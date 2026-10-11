/*
 * Lab Experiment 11: Multithreaded Array Sum using POSIX Threads (pthread)
 * 
 * Objective: Calculate the sum of a large integer array using POSIX threads.
 * Divide the array into approximately equal portions and assign each portion
 * to a separate thread. Each thread calculates and displays its local sum.
 * The main thread combines local sums and displays the overall total sum.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

// Structure to pass chunk parameters to each thread
typedef struct {
    int thread_id;
    int *array;
    int start_idx;
    int end_idx;
    long long local_sum;
} ThreadData;

// Thread worker function
void *calculate_sum(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->local_sum = 0;

    for (int i = data->start_idx; i < data->end_idx; i++) {
        data->local_sum += data->array[i];
    }

    printf("[Thread %d] Processed index [%7d to %7d] | Local Sum = %lld\n",
           data->thread_id, data->start_idx, data->end_idx - 1, data->local_sum);

    pthread_exit(NULL);
}

int main() {
    printf("==================================================\n");
    printf(" Experiment 11: Multithreaded Array Sum (pthreads)\n");
    printf("==================================================\n\n");

    printf("Allocating and initializing array of %d elements...\n", ARRAY_SIZE);
    int *arr = (int *)malloc(ARRAY_SIZE * sizeof(int));
    if (arr == NULL) {
        perror("Failed to allocate memory for array");
        exit(EXIT_FAILURE);
    }

    // Populate array: every element = 1 (Expected Total Sum = 1,000,000)
    for (int i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = 1;
    }

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    int chunk_size = ARRAY_SIZE / NUM_THREADS;
    int remainder = ARRAY_SIZE % NUM_THREADS;

    printf("Creating %d worker threads to compute partial sums...\n\n", NUM_THREADS);

    int current_start = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].array = arr;
        thread_data[i].start_idx = current_start;
        // Distribute remainder elements evenly across first few chunks
        int elements_in_chunk = chunk_size + (i < remainder ? 1 : 0);
        thread_data[i].end_idx = current_start + elements_in_chunk;
        current_start = thread_data[i].end_idx;

        int rc = pthread_create(&threads[i], NULL, calculate_sum, (void *)&thread_data[i]);
        if (rc != 0) {
            fprintf(stderr, "Error creating thread %d: return code %d\n", i, rc);
            free(arr);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to terminate and accumulate total sum
    long long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].local_sum;
    }

    printf("\n--------------------------------------------------\n");
    printf("All threads completed execution.\n");
    printf("Total Sum of %d array elements: %lld\n", ARRAY_SIZE, total_sum);
    printf("Verification: Expected Sum = %d | Status: %s\n",
           ARRAY_SIZE, (total_sum == ARRAY_SIZE) ? "PASSED" : "FAILED");
    printf("==================================================\n");

    free(arr);
    return 0;
}
