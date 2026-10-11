# OS Lab Viva Voce Questions and Answers (1 - 60)

---

### 1. What is the purpose of the `mkdir` and `touch` commands?
`mkdir` is used to create one or more new directories in the file system. `touch` is used to create empty files if they do not exist, or to update the access and modification timestamps of existing files.

---

### 2. What is the use of the `man` command in Linux? Give one example.
The `man` (manual) command displays the documentation and reference pages for Linux commands, system calls, and library functions.  
*Example:* `man 2 open` displays the manual page for the `open()` system call from section 2.

---

### 3. What is the purpose of `open()`, `read()`, and `write()` system calls?
`open()` establishes a connection to a file and returns a file descriptor for subsequent I/O. `read()` transfers raw bytes from an open file descriptor into a buffer, and `write()` transfers raw bytes from a buffer out to the file descriptor.

---

### 4. Why is `lseek()` used in the given file-handling program?
`lseek()` changes the current read/write file offset pointer associated with an open file descriptor. It enables random file access by repositioning the pointer directly to any specified byte location using `SEEK_SET`, `SEEK_CUR`, or `SEEK_END`.

---

### 5. What information is returned by the `readdir()` function?
`readdir()` reads the next directory entry from an open directory stream (`DIR*`) and returns a pointer to a `struct dirent`. It provides information including the file's inode number (`d_ino`) and filename (`d_name`).

---

### 6. Why is `closedir()` required after reading a directory?
`closedir()` closes the directory stream opened by `opendir()` and releases the allocated internal memory and underlying file descriptor back to the operating system, preventing resource and descriptor leaks.

---

### 7. What is the purpose of the `struct dirent` structure?
`struct dirent` represents an individual directory entry within a directory file. Its primary fields are `d_name` (the filename string) and `d_ino` (the inode number associated with that directory entry).

---

### 8. What is the difference between compiling a C program using `gcc filename.c` and `gcc filename.c -o filename`?
`gcc filename.c` compiles the program into the default executable named `a.out`. In contrast, `gcc filename.c -o filename` specifies a custom name for the resulting binary executable (e.g., `./filename`).

---

### 9. What is `errno` and when is it used?
`errno` is an integer global/thread-local variable defined in `<errno.h>` that stores error codes set by system calls and library functions upon failure. It is checked when a system call returns -1 to determine the exact cause of failure using `perror()` or `strerror(errno)`.

---

### 10. What is the purpose of the `fork()` system call?
`fork()` creates a new child process by duplicating the calling parent process. The child process receives an exact duplicate copy of the parent's address space, file descriptors, and CPU registers.

---

### 11. What is the difference between the return values of `fork()` in the parent and child processes?
In the parent process, `fork()` returns the PID of the newly created child process (> 0). In the child process, it returns `0`. If process creation fails, it returns `-1` to the parent.

---

### 12. Why is `wait()` used by the parent process?
`wait()` suspends execution of the parent process until one of its child processes terminates. It synchronizes execution, collects the child's termination status code, and reaps the child process to prevent it from becoming a zombie.

---

### 13. What is the purpose of `execvp()`?
`execvp()` replaces the current process image (code, data, heap, and stack) with a new executable program specified by filename, searching through the system `PATH` environment variable and passing an argument vector (`argv`).

---

### 14. What happens to the child process when an `exec` call is successful?
When `exec` succeeds, the child process does not return; its existing program memory is completely overlaid and replaced by the new program. The process retains the same PID, but starts execution from the entry point (`main`) of the new executable.

---

### 15. What is the basic principle of FCFS CPU scheduling?
First-Come, First-Served (FCFS) schedules processes non-preemptively strictly in the order they arrive in the ready queue. The process that requests the CPU first is allocated the CPU first and runs until its burst completes.

---

### 16. How is the waiting time of a process calculated in FCFS scheduling?
Waiting time is the total duration a process spends waiting in the ready queue before receiving CPU execution. It is calculated as:  
$$\text{Waiting Time (WT)} = \text{Turnaround Time (TAT)} - \text{Burst Time (BT)}$$  
(or $\text{WT}_i = \text{Start Time}_i - \text{Arrival Time}_i$).

---

### 17. What is the purpose of the time quantum in Round Robin scheduling?
The time quantum is a fixed slice of CPU time assigned to each process in Round Robin scheduling. It prevents CPU starvation, ensures fair CPU allocation, and provides responsive time-sharing across all ready processes.

---

### 18. What is a zombie process and how is it created?
A zombie process is a process that has completed execution via `exit()`, but whose entry still remains in the process table. It is created when its parent process has not yet invoked `wait()` or `waitpid()` to read its exit status and reap it.

---

### 19. What is an orphan process and how is it different from a zombie process?
An orphan process is a process whose parent has terminated while the child is still actively running; it is automatically adopted by `init` or `systemd` (PID 1). A zombie process has already finished executing but is waiting to be reaped by its living parent.

---

### 20. What is the difference between an anonymous pipe and a named pipe (FIFO)?
An anonymous pipe (`pipe()`) has no presence in the file system and can only be used between related processes (like parent and child). A named pipe (`mkfifo()`) appears as a special FIFO file in the file system and enables IPC between unrelated processes.

---

### 21. What is the purpose of the `pipe()` system call?
`pipe()` creates a unidirectional data communication channel (inter-process communication) in memory. It returns two file descriptors: `pipefd[0]` for reading data and `pipefd[1]` for writing data.

---

### 22. Why is `dup2()` used in the anonymous pipe program?
`dup2()` duplicates a file descriptor to a specific descriptor number, such as `STDIN_FILENO` (0) or `STDOUT_FILENO` (1). It redirects the standard input or standard output of a process to the pipe endpoints so standard I/O functions operate through the pipe.

---

### 23. What is the purpose of `mkfifo()`?
`mkfifo()` creates a named pipe (FIFO special file) in the filesystem with specified permission bits. Once created, unrelated processes can open it with `open()` like a standard file to stream data using FIFO semantics.

---

### 24. What happens if the reader and writer processes access a FIFO at different times?
By default, opening a FIFO for reading blocks until another process opens it for writing, and opening it for writing blocks until a reader opens it. Once both endpoints are open, reading from an empty FIFO blocks until data is written.

---

### 25. What is a signal in UNIX/Linux, and what is the purpose of `SIGINT`?
A signal is an asynchronous software interrupt delivered by the OS kernel to notify a process of an event. `SIGINT` (Signal 2) is sent to an active process when the user presses `Ctrl+C` to request process interruption.

---

### 26. What is the purpose of `SIGCHLD`?
`SIGCHLD` is a signal sent automatically by the Linux kernel to a parent process whenever one of its child processes terminates, stops, or resumes execution.

---

### 27. Why is `wait()` used inside the `SIGCHLD` handler?
Using `wait()` (or `waitpid()` with `WNOHANG`) inside the `SIGCHLD` handler allows the parent process to asynchronously harvest the child's exit status immediately upon termination, preventing child processes from accumulating as zombie processes.

---

### 28. What is the purpose of `shmget()` and `shmat()` in shared memory communication?
`shmget()` allocates or accesses a System V shared memory segment and returns a shared memory identifier. `shmat()` attaches this shared memory segment to the address space of the calling process so it can access it as a pointer.

---

### 29. Why is `shmctl()` with `IPC_RMID` used in the shared-memory program?
`shmctl()` with the command flag `IPC_RMID` marks the shared memory segment for destruction and removal from the kernel once all attached processes detach from it, preventing memory leaks in shared IPC memory.

---

### 30. What information is returned by the `stat()` system call?
`stat()` returns file metadata into a `struct stat`, including inode number (`st_ino`), file type and permissions (`st_mode`), size in bytes (`st_size`), owner UID (`st_uid`), group GID (`st_gid`), hard-link count (`st_nlink`), and timestamps (`st_atime`, `st_mtime`, `st_ctime`).

---

### 31. What is an inode number and what does it identify?
An inode number is a unique integer identifier within a file system that points to an inode data structure. It identifies the file's metadata and disk block locations, independent of its human-readable directory filename.

---

### 32. How can two filenames refer to the same underlying file?
Two filenames refer to the same underlying file by creating a hard link using `link()`. Both directory entries map different name strings to the identical inode number on the storage volume.

---

### 33. What is the significance of the `st_nlink` field in `struct stat`?
The `st_nlink` field stores the hard-link count of the file, representing how many directory entries point to this inode. A file's storage blocks are freed only when its link count drops to zero and no process holds it open.

---

### 34. What is the difference between `stat` and the Linux command `ls -li` for displaying file information?
`stat` is a system call (and command) that provides exhaustive metadata about an individual file (exact nanosecond timestamps, blocks, device ID, inode). `ls -li` is a user-level directory listing command that prints formatted summary lines including inode numbers for multiple files.

---

### 35. What is the difference between a hard link and a symbolic link?
A hard link is a direct directory reference sharing the exact same inode and file data as the original file. A symbolic (soft) link is a distinct file with its own inode containing a text path pointing to the target file.

---

### 36. Why does a hard link normally have the same inode number as the original file?
Because a hard link does not duplicate the file's contents or inode; it simply adds another directory entry name pointing to the existing inode data structure on the same file system.

---

### 37. Why is `lstat()` used for the symbolic link instead of `stat()`?
`stat()` automatically follows (dereferences) a symbolic link and returns the metadata of the target file it points to. `lstat()` does not follow the link; it returns the metadata of the symbolic link file itself.

---

### 38. What is the purpose of the `unlink()` system call?
`unlink()` removes a directory entry (filename link) from the file system and decrements the inode's hard-link count (`st_nlink`). If the count reaches zero and no process has it open, the file's storage is deallocated.

---

### 39. What happens to the original file's link count when a hard link is created and subsequently deleted?
When a hard link is created, the link count (`st_nlink`) increments by 1 (e.g., from 1 to 2). When the hard link is deleted with `unlink()`, the link count decrements back by 1 (e.g., from 2 back to 1).

---

### 40. What is the purpose of the `open()` system call and what does it return?
`open()` creates or accesses a file and prepares it for I/O operations with specified access modes (`O_RDONLY`, `O_WRONLY`, `O_RDWR`, etc.). It returns a non-negative integer file descriptor on success, or `-1` on failure.

---

### 41. What is the difference between sequential and random file access?
Sequential file access reads or writes file contents consecutively from beginning to end in byte order. Random file access allows direct navigation to any arbitrary byte offset using `lseek()` without reading preceding bytes.

---

### 42. Why is `lseek()` used in file handling?
`lseek()` repositions the file offset pointer of an open file descriptor. It is used to perform non-sequential operations such as seeking records, skipping headers, or appending data relative to the start, current position, or end of a file.

---

### 43. What is an inode and how can its value be obtained programmatically?
An inode (index node) is a filesystem data structure storing all file metadata except its name. Its value is obtained programmatically by invoking the `stat()` or `lstat()` system call and reading the `st_ino` member of `struct stat`.

---

### 44. What information can be obtained using the `stat()` system call?
`stat()` provides the file's inode number, file type (regular, directory, symlink, etc.), file size in bytes, read/write/execute permissions, owner UID, group GID, hard-link count, and timestamps for access, modification, and status change.

---

### 45. What is a POSIX thread and how is it different from a process?
A POSIX thread (pthread) is an independent execution unit within a process. While processes have separate virtual memory address spaces and require IPC to communicate, threads of the same process share the same memory (heap, globals, file descriptors) but have their own execution stack and registers.

---

### 46. What is the purpose of `pthread_create()`?
`pthread_create()` spawns a new thread within the calling process. It takes attributes, the thread worker function pointer, and arguments, and begins executing the function concurrently with the caller.

---

### 47. Why is `pthread_join()` required in the array-summation program?
`pthread_join()` blocks the calling main thread until the specified worker thread completes execution. It synchronizes execution, collects the thread's termination status, and guarantees all partial sums are computed before calculating the grand total.

---

### 48. Why is the array divided into separate portions among the threads?
Dividing the array into separate contiguous segments allows worker threads to process distinct memory ranges concurrently on multicore CPUs without data conflicts or locking overhead, achieving parallel speedup.

---

### 49. How does the program handle the remainder when the array size is not evenly divisible by the number of threads?
The base chunk size is calculated as $\text{size} / \text{threads}$ and the remainder $\text{size} \pmod{\text{threads}}$ is distributed by adding 1 extra element to each of the first remainder threads, ensuring all elements are covered.

---

### 50. What is the purpose of `fork()` in the parallel log analyzer?
`fork()` divides the heavy log file processing workload across two separate process address spaces (parent and child), allowing them to independently read and analyze partitioned sections of the log file in parallel.

---

### 51. Why are POSIX threads used inside each process?
Threads are used within each process to perform concurrent keyword searches (`INFO` in one thread and `ERROR` in another) on that process's allocated log partition, utilizing multicore processing while the main thread counts `WARNING` entries.

---

### 52. What is the purpose of `pthread_join()`?
`pthread_join()` waits for a thread specified by thread ID to terminate. It acts as a synchronization barrier, cleans up thread resources, and retrieves any return value passed to `pthread_exit()`.

---

### 53. Why can the child process not directly modify the parent's normal variables?
Because `fork()` provides the child process with a private copy of the parent's address space. Due to virtual memory isolation and copy-on-write, any modification made by the child affects only its own memory page, not the parent's.

---

### 54. Why is a pipe required for communication between the child and parent processes?
Because parent and child processes execute in isolated address spaces, a pipe (`pipe()`) provides a shared kernel-buffered channel allowing the child process to transmit its local message counts back to the parent for final consolidation.

---

### 55. What is a race condition? Give an example from the bank transaction program.
A race condition occurs when multiple concurrent threads access and modify shared data simultaneously, causing the final outcome to depend on non-deterministic thread scheduling.  
*Example:* Two threads simultaneously check and deduct from the same bank balance variable, causing inconsistent balance updates and incorrect deductions.

---

### 56. What is a critical section? Identify it in the bank withdrawal program.
A critical section is a block of code accessing shared resources that must not be executed concurrently by more than one thread. In the bank withdrawal program, the critical section is the code that checks if `balance >= amount` and decrements `balance = balance - amount`.

---

### 57. What is the purpose of `pthread_mutex_lock()`?
`pthread_mutex_lock()` acquires exclusive ownership of a mutex. If the mutex is already locked by another thread, the calling thread blocks until the mutex becomes available, ensuring mutual exclusion.

---

### 58. What is the difference between `pthread_mutex_lock()` and `pthread_mutex_unlock()`?
`pthread_mutex_lock()` acquires the lock before entering a critical section (blocking if unavailable), whereas `pthread_mutex_unlock()` releases the held lock upon exiting the critical section so other waiting threads can acquire it.

---

### 59. Why are `pthread_mutex_init()` and `pthread_mutex_destroy()` required?
`pthread_mutex_init()` initializes the mutex object and sets its attributes before use. `pthread_mutex_destroy()` cleans up and invalidates the mutex once threads are done with it, releasing any system resources allocated to the synchronization object.
