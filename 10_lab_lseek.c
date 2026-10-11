/*
 * Lab Experiment 10: Random File Access using lseek() and File Metadata via stat()
 * 
 * Objective:
 *   1. Extend file I/O to demonstrate random file access using lseek().
 *   2. Move the file offset to a specified byte position and read data from that position.
 *   3. Use stat() system call to display:
 *      - Inode number
 *      - File size
 *      - Number of links
 *      - User ID (UID)
 *      - Group ID (GID)
 *      - File mode (permissions and type)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE 256

void display_file_stat(const char *filename) {
    struct stat st;

    printf("\n--------------------------------------------------\n");
    printf(" File Metadata using stat(): '%s'\n", filename);
    printf("--------------------------------------------------\n");

    if (stat(filename, &st) == -1) {
        perror("stat() failed");
        return;
    }

    printf("  Inode Number:        %lu\n", (unsigned long)st.st_ino);
    printf("  File Size:           %ld bytes\n", (long)st.st_size);
    printf("  Number of Links:     %lu\n", (unsigned long)st.st_nlink);
    printf("  User ID (UID):       %u\n", (unsigned int)st.st_uid);
    printf("  Group ID (GID):      %u\n", (unsigned int)st.st_gid);
    printf("  File Mode:           0%o (octal permissions)\n", st.st_mode & 0777);
    printf("--------------------------------------------------\n\n");
}

int main() {
    const char *filename = "random_access_demo.txt";
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    printf("==================================================\n");
    printf(" Experiment 10: Random File Access (lseek) & stat\n");
    printf("==================================================\n\n");

    // 1. Create file and populate with known indexed records
    fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open() failed");
        exit(EXIT_FAILURE);
    }

    // Fixed-length structured records (each exactly 20 characters: 19 chars + '\n')
    // 0123456789012345678\n
    // Record 0: Item-AAAA\n -> 19 chars + 1 = 20 bytes
    // Record 1: Item-BBBB\n -> 19 chars + 1 = 20 bytes
    // Record 2: Item-CCCC\n -> 19 chars + 1 = 20 bytes
    // Record 3: Item-DDDD\n -> 19 chars + 1 = 20 bytes
    const char *data =
        "Record 0: Item-AAAA\n"
        "Record 1: Item-BBBB\n"
        "Record 2: Item-CCCC\n"
        "Record 3: Item-DDDD\n";

    if (write(fd, data, strlen(data)) == -1) {
        perror("write() failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("[Setup] Created '%s' with 4 structured records (20 bytes each).\n", filename);

    // 2. Display file stat metadata
    display_file_stat(filename);

    // 3. Demonstrate lseek(): Seek to Record 2 (offset 40 from SEEK_SET)
    off_t target_offset = 40;
    off_t current_offset = lseek(fd, target_offset, SEEK_SET);
    if (current_offset == (off_t)-1) {
        perror("lseek() SEEK_SET failed");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("[lseek] Repositioned offset to %ld bytes from start (SEEK_SET).\n", (long)current_offset);

    memset(buffer, 0, sizeof(buffer));
    bytes_read = read(fd, buffer, 19); // Read 19 bytes (Record 2)
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("[Read at offset %ld]: \"%s\"\n\n", (long)target_offset, buffer);
    }

    // 4. Demonstrate SEEK_CUR: skip ahead 1 byte (newline) and read Record 3
    current_offset = lseek(fd, 1, SEEK_CUR);
    printf("[lseek] Advanced offset by 1 byte from current position (SEEK_CUR) -> Now at %ld.\n", (long)current_offset);
    memset(buffer, 0, sizeof(buffer));
    bytes_read = read(fd, buffer, 19);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("[Read at offset %ld]: \"%s\"\n\n", (long)current_offset, buffer);
    }

    // 5. Demonstrate SEEK_END: Go back 20 bytes from EOF to read Record 3 again
    current_offset = lseek(fd, -20, SEEK_END);
    printf("[lseek] Repositioned to -20 bytes from EOF (SEEK_END) -> Now at %ld.\n", (long)current_offset);
    memset(buffer, 0, sizeof(buffer));
    bytes_read = read(fd, buffer, 19);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("[Read at offset %ld]: \"%s\"\n\n", (long)current_offset, buffer);
    }

    close(fd);
    printf("[Clean-up] File closed. Random access demonstration finished.\n");
    printf("==================================================\n");

    return 0;
}
