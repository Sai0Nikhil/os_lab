# Operating Systems Laboratory Programs

This repository contains POSIX compliant C implementations for the Operating Systems Laboratory experiments.

---

## Index of Experiments

| File | Question / Topic | Description & System Calls Used |
| :--- | :--- | :--- |
| [`1_lab_open.c`](1_lab_open.c) | **Q1: File Opening & Error Handling** | Demonstrates `open()`, `close()`, `errno`, `strerror(errno)`, and `perror()` upon opening a non-existent file. |
| [`2_lab_fork.c`](2_lab_fork.c) | **Q2: Process Lifecycle & States** | Demonstrates process creation (`fork()`), PID queries (`getpid()`), blocking state (`sleep()`), and parent synchronization (`wait()`). States: NEW, READY, RUNNING, WAITING, TERMINATED. |
| [`3_lab_fcfs.c`](3_lab_fcfs.c) | **Q3: CPU Scheduling (FCFS)** | First-Come First-Served scheduling calculating Completion Time, Waiting Time, Turnaround Time, averages, and ASCII Gantt Chart. |
| [`5_lab_pipe.c`](5_lab_pipe.c) | **Q5: IPC using Pipe & dup2()** | Inter-process communication redirecting Parent `stdout` to pipe via `dup2()` and Child `stdin` from pipe via `dup2()`. |
| [`6_lab_signal.c`](6_lab_signal.c) | **Q6: Signal Handling** | Demonstrates `SIGINT` (interruption) handler and `SIGCHLD` asynchronous child termination detection with non-blocking `waitpid()`. |
| [`7_lab_stat.c`](7_lab_stat.c) | **Q7: File Metadata using stat()** | Accepts CLI filenames and inspects inode, file type, size, permissions (rwx/octal), UID, GID, link count, and timestamps (atime, mtime, ctime). |
| [`8_lab_links.c`](8_lab_links.c) | **Q8: Hard Links & Symbolic Links** | Creates hard link (`link()`) and soft link (`symlink()`), displaying inode numbers and link counts using `stat()` vs `lstat()`. |
| [`9_lab_file_io.c`](9_lab_file_io.c) | **Q9: Sequential File I/O** | POSIX file I/O using `open()`, `write()`, `read()`, and `close()` for student records with error handling. |
| [`10_lab_lseek.c`](10_lab_lseek.c) | **Q10: Random File Access & Metadata** | Random file access using `lseek()` with `SEEK_SET`, `SEEK_CUR`, and `SEEK_END`, plus file metadata via `stat()`. |
| [`11_lab_pthread_sum.c`](11_lab_pthread_sum.c) | **Q11: Multithreaded Array Sum** | POSIX Threads (`pthread`) dividing a 1,000,000 element array among 4 worker threads to calculate and display local and total sums. |
| [`12_lab_log_analyzer.c`](12_lab_log_analyzer.c) | **Q12: Concurrent Log Analyzer** | Multiprocess (`fork()`) and multithreaded (`pthread`) log analyzer counting `INFO`, `WARNING`, and `ERROR` log messages with IPC aggregation. |

---

## Compilation and Execution

Compile all programs with `gcc`:

```bash
# Lab 1
gcc -Wall -Wextra 1_lab_open.c -o 1_lab_open && ./1_lab_open

# Lab 2
gcc -Wall -Wextra 2_lab_fork.c -o 2_lab_fork && ./2_lab_fork

# Lab 3
gcc -Wall -Wextra 3_lab_fcfs.c -o 3_lab_fcfs && ./3_lab_fcfs

# Lab 5
gcc -Wall -Wextra 5_lab_pipe.c -o 5_lab_pipe && ./5_lab_pipe

# Lab 6
gcc -Wall -Wextra 6_lab_signal.c -o 6_lab_signal && ./6_lab_signal

# Lab 7
gcc -Wall -Wextra 7_lab_stat.c -o 7_lab_stat && ./7_lab_stat 7_lab_stat.c

# Lab 8
gcc -Wall -Wextra 8_lab_links.c -o 8_lab_links && ./8_lab_links

# Lab 9
gcc -Wall -Wextra 9_lab_file_io.c -o 9_lab_file_io && ./9_lab_file_io

# Lab 10
gcc -Wall -Wextra 10_lab_lseek.c -o 10_lab_lseek && ./10_lab_lseek

# Lab 11
gcc -Wall -Wextra 11_lab_pthread_sum.c -o 11_lab_pthread_sum -pthread && ./11_lab_pthread_sum

# Lab 12
gcc -Wall -Wextra 12_lab_log_analyzer.c -o 12_lab_log_analyzer -pthread && ./12_lab_log_analyzer
```

## Generated Lab Manual (DOCX)
The complete lab manual containing questions, algorithms/explanations, full C source code, and sample terminal execution outputs is available in **`OS_Lab_Programs.docx`**.
