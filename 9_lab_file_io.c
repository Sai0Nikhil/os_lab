/*
 * Lab Experiment 9: Sequential File I/O using POSIX System Calls
 * 
 * Objective: Use open(), write(), read(), and close() to:
 *   1. Create a file.
 *   2. Write student/academic data into it.
 *   3. Sequentially read and display the contents.
 *   4. Appropriately handle file opening, reading, and writing errors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 256

int main() {
    const char *filename = "student_records.txt";
    int fd;
    ssize_t bytes_written, bytes_read;
    char buffer[BUFFER_SIZE];

    printf("==================================================\n");
    printf(" Experiment 9: Sequential File I/O using POSIX Calls\n");
    printf("==================================================\n\n");

    // Student records to write
    const char *student_data =
        "Roll No: 101 | Name: Sai Nikhil | Course: Operating Systems | Grade: A+\n"
        "Roll No: 102 | Name: Alice Smith | Course: Computer Networks | Grade: A\n"
        "Roll No: 103 | Name: Bob Johnson | Course: Database Systems   | Grade: B+\n";

    // 1. Create file and open for writing (O_CREAT | O_WRONLY | O_TRUNC)
    printf("[Step 1] Creating and opening '%s' for writing...\n", filename);
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "[ERROR] open() for writing failed: %s (errno: %d)\n", strerror(errno), errno);
        exit(EXIT_FAILURE);
    }
    printf("[SUCCESS] File descriptor obtained: %d\n\n", fd);

    // 2. Write data to the file
    printf("[Step 2] Writing student academic records into file...\n");
    size_t data_len = strlen(student_data);
    bytes_written = write(fd, student_data, data_len);
    if (bytes_written == -1) {
        fprintf(stderr, "[ERROR] write() failed: %s (errno: %d)\n", strerror(errno), errno);
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("[SUCCESS] %zd bytes successfully written to file.\n\n", bytes_written);

    // 3. Close the file after writing
    if (close(fd) == -1) {
        fprintf(stderr, "[ERROR] close() after writing failed: %s (errno: %d)\n", strerror(errno), errno);
        exit(EXIT_FAILURE);
    }
    printf("[SUCCESS] File closed successfully after writing.\n\n");

    // 4. Open the file for reading (O_RDONLY)
    printf("[Step 3] Opening '%s' in read-only mode to verify contents...\n", filename);
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "[ERROR] open() for reading failed: %s (errno: %d)\n", strerror(errno), errno);
        exit(EXIT_FAILURE);
    }

    // 5. Read sequentially in chunks and display
    printf("[Step 4] Sequentially reading file contents:\n");
    printf("--------------------------------------------------------------------------\n");
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0'; // Null-terminate the buffer
        printf("%s", buffer);
    }
    printf("--------------------------------------------------------------------------\n");

    if (bytes_read == -1) {
        fprintf(stderr, "[ERROR] read() failed: %s (errno: %d)\n", strerror(errno), errno);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 6. Close the file after reading
    if (close(fd) == -1) {
        fprintf(stderr, "[ERROR] close() after reading failed: %s (errno: %d)\n", strerror(errno), errno);
        exit(EXIT_FAILURE);
    }
    printf("[SUCCESS] File closed successfully. Sequential I/O demonstration finished.\n");
    printf("==================================================\n");

    return 0;
}
