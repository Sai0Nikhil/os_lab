/*
 * Lab Experiment 7: File Attributes Inspection using stat()
 * 
 * Objective: Accept one or more filenames as command-line arguments.
 * Use stat() system call to display:
 *   - Inode number
 *   - File type
 *   - File size
 *   - Permissions
 *   - Owner ID (UID)
 *   - Group ID (GID)
 *   - Hard-link count
 *   - Access, modification, and status-change timestamps
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <string.h>

void print_file_type(mode_t mode) {
    printf("  File Type:               ");
    if (S_ISREG(mode))  printf("Regular File (-)\n");
    else if (S_ISDIR(mode))  printf("Directory (d)\n");
    else if (S_ISCHR(mode))  printf("Character Device (c)\n");
    else if (S_ISBLK(mode))  printf("Block Device (b)\n");
    else if (S_ISFIFO(mode)) printf("FIFO / Pipe (p)\n");
    else if (S_ISLNK(mode))  printf("Symbolic Link (l)\n");
    else if (S_ISSOCK(mode)) printf("Socket (s)\n");
    else                     printf("Unknown\n");
}

void print_permissions(mode_t mode) {
    char perms[10];
    perms[0] = (mode & S_IRUSR) ? 'r' : '-';
    perms[1] = (mode & S_IWUSR) ? 'w' : '-';
    perms[2] = (mode & S_IXUSR) ? 'x' : '-';
    perms[3] = (mode & S_IRGRP) ? 'r' : '-';
    perms[4] = (mode & S_IWGRP) ? 'w' : '-';
    perms[5] = (mode & S_IXGRP) ? 'x' : '-';
    perms[6] = (mode & S_IROTH) ? 'r' : '-';
    perms[7] = (mode & S_IWOTH) ? 'w' : '-';
    perms[8] = (mode & S_IXOTH) ? 'x' : '-';
    perms[9] = '\0';
    printf("  Permissions:             %s (octal %o)\n", perms, mode & 0777);
}

void display_stat_info(const char *filename) {
    struct stat file_stat;
    char time_buf[64];

    printf("=================================================================\n");
    printf(" File Attributes for: %s\n", filename);
    printf("=================================================================\n");

    if (stat(filename, &file_stat) == -1) {
        fprintf(stderr, "Error accessing '%s': %s (errno: %d)\n\n",
                filename, strerror(errno), errno);
        return;
    }

    printf("  Inode Number:            %lu\n", (unsigned long)file_stat.st_ino);
    print_file_type(file_stat.st_mode);
    printf("  File Size:               %ld bytes\n", (long)file_stat.st_size);
    print_permissions(file_stat.st_mode);
    printf("  Owner User ID (UID):     %u\n", (unsigned int)file_stat.st_uid);
    printf("  Group ID (GID):          %u\n", (unsigned int)file_stat.st_gid);
    printf("  Hard-link Count:         %lu\n", (unsigned long)file_stat.st_nlink);

    // Timestamps
    struct tm *tm_info;

    tm_info = localtime(&file_stat.st_atime);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("  Last Access Time:        %s\n", time_buf);

    tm_info = localtime(&file_stat.st_mtime);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("  Last Modification Time:  %s\n", time_buf);

    tm_info = localtime(&file_stat.st_ctime);
    strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("  Last Status Change Time: %s\n", time_buf);

    printf("-----------------------------------------------------------------\n\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename1> [filename2 ...]\n", argv[0]);
        printf("Demonstrating on current source file and current directory:\n\n");
        display_stat_info(argv[0]);
        display_stat_info(".");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        display_stat_info(argv[i]);
    }

    return 0;
}
