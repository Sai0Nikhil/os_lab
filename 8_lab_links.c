/*
 * Lab Experiment 8: Hard Links and Symbolic Links
 * 
 * Objective: Create a hard link using link() and a symbolic link using symlink().
 * Display the inode number and link count of:
 *   - Original file
 *   - Hard link
 *   - Symbolic link
 * Use stat() and lstat() appropriately to distinguish the symbolic link from its target.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

void display_info(const char *label, const char *path, int use_lstat) {
    struct stat st;
    int ret;

    if (use_lstat) {
        ret = lstat(path, &st);
    } else {
        ret = stat(path, &st);
    }

    if (ret == -1) {
        perror(path);
        return;
    }

    printf("%-20s | Path: %-20s | Inode: %-10lu | Links: %-4lu | Type: %s\n",
           label,
           path,
           (unsigned long)st.st_ino,
           (unsigned long)st.st_nlink,
           S_ISLNK(st.st_mode) ? "Symbolic Link" : (S_ISREG(st.st_mode) ? "Regular File" : "Other"));
}

int main() {
    const char *orig_file = "orig_file.txt";
    const char *hard_link = "hard_link.txt";
    const char *soft_link = "soft_link.txt";

    printf("========================================================================================\n");
    printf(" Experiment 8: Hard Link and Symbolic Link Demonstration\n");
    printf("========================================================================================\n\n");

    // Clean up any old files from previous runs
    unlink(orig_file);
    unlink(hard_link);
    unlink(soft_link);

    // 1. Create original file
    int fd = open(orig_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to create original file");
        exit(EXIT_FAILURE);
    }
    const char *content = "Data inside original file.\n";
    write(fd, content, strlen(content));
    close(fd);
    printf("[Step 1] Created original file: '%s'\n", orig_file);

    // 2. Create hard link
    if (link(orig_file, hard_link) == -1) {
        perror("link() failed");
    } else {
        printf("[Step 2] Created hard link '%s' -> '%s' using link()\n", hard_link, orig_file);
    }

    // 3. Create symbolic link
    if (symlink(orig_file, soft_link) == -1) {
        perror("symlink() failed");
    } else {
        printf("[Step 3] Created symbolic link '%s' -> '%s' using symlink()\n\n", soft_link, orig_file);
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("File Attributes Comparison:\n");
    printf("----------------------------------------------------------------------------------------\n");
    display_info("Original File", orig_file, 0);
    display_info("Hard Link", hard_link, 0);
    display_info("Soft Link (lstat)", soft_link, 1);
    display_info("Soft Link (stat)", soft_link, 0);
    printf("----------------------------------------------------------------------------------------\n\n");

    printf("Key Observations:\n");
    printf(" 1. Hard link shares the EXACT SAME inode number and file data as the original file.\n");
    printf(" 2. Link count of original file increases from 1 to 2 after creating the hard link.\n");
    printf(" 3. Symbolic link has its OWN UNIQUE inode number and link count is 1.\n");
    printf(" 4. lstat() returns information about the symbolic link itself (Type: Symbolic Link).\n");
    printf(" 5. stat() follows the symbolic link and returns info of the target file (Type: Regular File).\n");

    // Cleanup
    unlink(orig_file);
    unlink(hard_link);
    unlink(soft_link);
    printf("\nTemporary link test files cleaned up successfully.\n");
    printf("========================================================================================\n");

    return 0;
}
