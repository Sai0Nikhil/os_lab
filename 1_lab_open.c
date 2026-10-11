/*
 * Lab Experiment 1: File Opening and Error Handling
 * 
 * Objective: Demonstrate open(), close(), errno, strerror(), and perror().
 * Attempt to open a non-existent file and display:
 *   (i) error number using errno
 *   (ii) descriptive error message using strerror(errno)
 *   (iii) error message using perror()
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
    const char *filename = "non_existent_file_xyz.txt";
    
    printf("==================================================\n");
    printf(" Experiment 1: File Opening and Error Handling\n");
    printf("==================================================\n\n");
    
    printf("Attempting to open file: '%s' in read-only mode...\n\n", filename);
    
    // Attempting to open non-existent file
    int fd = open(filename, O_RDONLY);
    
    if (fd == -1) {
        printf("[FAILURE] open() system call failed as expected.\n\n");
        
        // (i) Error number using errno
        printf("(i) Error number (errno): %d\n", errno);
        
        // (ii) Descriptive error message using strerror(errno)
        printf("(ii) Error description using strerror(): %s\n", strerror(errno));
        
        // (iii) Error reporting using perror()
        printf("(iii) Error reporting using perror():\n     ");
        perror("open() failed");
    } else {
        printf("[SUCCESS] File opened successfully with file descriptor: %d\n", fd);
        close(fd);
    }
    
    // Demonstrating successful file creation and opening
    printf("\n--------------------------------------------------\n");
    printf("Now creating and opening a file successfully...\n");
    const char *new_file = "sample_test.txt";
    fd = open(new_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to create file");
    } else {
        printf("[SUCCESS] Created '%s' with file descriptor: %d\n", new_file, fd);
        if (close(fd) == 0) {
            printf("[SUCCESS] File descriptor %d successfully closed using close().\n", fd);
        } else {
            perror("Failed to close file");
        }
        unlink(new_file); // Clean up temporary file
    }
    
    printf("==================================================\n");
    return 0;
}
