import docx
from docx.shared import Inches, Pt, RGBColor
from docx.enum.text import WD_ALIGN_PARAGRAPH
from docx.enum.table import WD_TABLE_ALIGNMENT
from docx.oxml import OxmlElement, parse_xml
from docx.oxml.ns import nsdecls, qn

doc = docx.Document()

# Page Margins
sections = doc.sections
for section in sections:
    section.top_margin = Inches(0.8)
    section.bottom_margin = Inches(0.8)
    section.left_margin = Inches(0.8)
    section.right_margin = Inches(0.8)

# Color constants
PRIMARY_COLOR = RGBColor(24, 76, 120)    # Deep Steel Blue
SECONDARY_COLOR = RGBColor(41, 128, 185) # Vivid Blue
CODE_BG = "F4F6F7"
OUTPUT_BG = "2C3E50"

# Title
title_p = doc.add_paragraph()
title_p.alignment = WD_ALIGN_PARAGRAPH.CENTER
title_run = title_p.add_run("OPERATING SYSTEMS LABORATORY")
title_run.bold = True
title_run.font.size = Pt(22)
title_run.font.name = "Arial"
title_run.font.color.rgb = PRIMARY_COLOR

subtitle_p = doc.add_paragraph()
subtitle_p.alignment = WD_ALIGN_PARAGRAPH.CENTER
subtitle_run = subtitle_p.add_run("Comprehensive Lab Experiments & POSIX Implementations in C\n")
subtitle_run.font.size = Pt(13)
subtitle_run.font.italic = True
subtitle_run.font.color.rgb = RGBColor(100, 100, 100)

doc.add_paragraph("─" * 70)

experiments = [
    {
        "id": "Q1",
        "title": "File Opening and Error Handling",
        "filename": "1_lab_open.c",
        "description": "Write C programs to demonstrate file opening and error handling using open(), close(), errno, and strerror(). Attempt to open a file that does not exist and display (i) the error number using errno, and (ii) the corresponding descriptive error message using strerror(errno). Also demonstrate appropriate error reporting using perror().",
        "concepts": "POSIX open(), close(), errno global error indicator, strerror(errno), perror() function.",
        "sample_output": """==================================================
 Experiment 1: File Opening and Error Handling
==================================================

Attempting to open file: 'non_existent_file_xyz.txt' in read-only mode...

[FAILURE] open() system call failed as expected.

(i) Error number (errno): 2
(ii) Error description using strerror(): No such file or directory
(iii) Error reporting using perror():
     open() failed: No such file or directory

--------------------------------------------------
Now creating and opening a file successfully...
[SUCCESS] Created 'sample_test.txt' with file descriptor: 10
[SUCCESS] File descriptor 10 successfully closed using close().
=================================================="""
    },
    {
        "id": "Q2",
        "title": "Process Life Cycle and Synchronization",
        "filename": "2_lab_fork.c",
        "description": "Write and execute a C program using fork(), getpid(), sleep(), and wait() to demonstrate parent and child process creation and the process life cycle. Display the NEW, READY, RUNNING, WAITING, and TERMINATED states of the child process and demonstrate synchronization between parent and child.",
        "concepts": "Process states (NEW, READY, RUNNING, WAITING, TERMINATED), fork(), getpid(), getppid(), sleep(), wait(), WIFEXITED, WEXITSTATUS macros.",
        "sample_output": """==================================================
 Experiment 2: Process Life Cycle & Synchronization
==================================================

[Parent] Main process starting. PID = 11510
[State: NEW] fork() is about to be invoked to create a child process...

[Parent] Created child process with PID = 11516
[Parent: WAITING] Parent synchronizing with child using wait()...
----------------- CHILD PROCESS -----------------
[Child: READY -> RUNNING] Child is now executing!
[Child] PID = 11516, Parent PID = 11510
[Child: RUNNING -> WAITING] Child going to sleep for 2 seconds (simulating I/O / blocking)...
[Child: WAITING -> READY -> RUNNING] Child woke up from sleep.
[Child: RUNNING -> TERMINATED] Child performing exit with exit code 42.
-------------------------------------------------

[Parent: RUNNING] wait() returned! Child (PID: 11516) has terminated.
[Parent] Child process terminated normally with exit status: 42
[Parent] Parent completing execution and terminating.
=================================================="""
    },
    {
        "id": "Q3",
        "title": "First-Come, First-Served (FCFS) CPU Scheduling Algorithm",
        "filename": "3_lab_fcfs.c",
        "description": "Implement the First-Come, First-Served (FCFS) CPU scheduling algorithm. Accept the number of processes and their burst times as input, calculate the waiting time and turnaround time for every process, and determine the average waiting time and average turnaround time.",
        "concepts": "CPU Scheduling, Non-preemptive scheduling, Completion Time (CT), Turnaround Time (TAT = CT - AT), Waiting Time (WT = TAT - BT), Gantt Chart.",
        "sample_output": """==================================================
 Experiment 3: First-Come, First-Served (FCFS) CPU Scheduling
==================================================

Enter number of processes: 3
Enter burst time for Process P1: 5
Enter burst time for Process P2: 8
Enter burst time for Process P3: 12

---------------------------------------------------------------------------
| Process | Burst Time (BT) | Waiting Time (WT) | Turnaround Time (TAT) |
---------------------------------------------------------------------------
|   P1    |      5          |        0          |          5           |
|   P2    |      8          |        5          |          13          |
|   P3    |      12         |        13         |          25          |
---------------------------------------------------------------------------

Gantt Chart:
+-------+-------+-------+
|  P1   |  P2   |  P3   |
+-------+-------+-------+
0      5       13      25

Total Waiting Time     = 18.00
Average Waiting Time   = 6.00
Total Turnaround Time  = 43.00
Average Turnaround Time = 14.33
=================================================="""
    },
    {
        "id": "Q5",
        "title": "Inter-Process Communication using Anonymous Pipe and dup2()",
        "filename": "5_lab_pipe.c",
        "description": "Write a C program to demonstrate inter-process communication using an anonymous pipe. Create a parent and child process using fork(). Use pipe() and dup2() to redirect the parent's standard output to the pipe and the child's standard input from the pipe. Transfer a message from the parent process to the child process and display the received message.",
        "concepts": "IPC Anonymous Pipe (pipe()), File descriptor manipulation (dup(), dup2()), Standard Streams Redirection (STDIN_FILENO, STDOUT_FILENO).",
        "sample_output": """==================================================
 Experiment 5: IPC using Anonymous Pipe and dup2()
==================================================

[Parent Process] Message sent to child via redirected stdout.
[Parent Process] Waiting for child process to finish...
[Child Process] Successfully read message from redirected stdin:
                "Hello from Parent process! IPC via pipe and dup2() succeeded."
[Parent Process] Child finished. IPC demonstration complete.
=================================================="""
    },
    {
        "id": "Q6",
        "title": "Signal Handling: SIGINT and SIGCHLD with Asynchronous Wait",
        "filename": "6_lab_signal.c",
        "description": "Write a C program to demonstrate signal handling. Register a signal handler for SIGINT and display an appropriate message when the signal is received. Also implement a parent-child program using SIGCHLD to detect child-process termination and perform the required wait() operation.",
        "concepts": "POSIX Signal Handling, sigaction structure, SIGINT catching, SIGCHLD asynchronous notifications, non-blocking waitpid(WNOHANG) preventing zombie processes.",
        "sample_output": """==================================================
 Experiment 6: Signal Handling (SIGINT & SIGCHLD)
==================================================

[Setup] SIGINT handler registered. (Try pressing Ctrl+C or kill -SIGINT 11817)
[Setup] SIGCHLD handler registered.

Sending SIGINT to current process via raise(SIGINT)...

[SIGNAL HANDLER] SIGINT (Signal 2) caught! Interruption handled gracefully.

Creating child process to demonstrate SIGCHLD detection...
[Parent: PID 11817] Child spawned (PID 11823). Parent continuing execution...
[Parent] Waiting for SIGCHLD event... (sleeping 1s)
  [Child: PID 11823] Running and doing work for 2 seconds...
[Parent] Waiting for SIGCHLD event... (sleeping 1s)
  [Child: PID 11823] Finished work, terminating now with status 10.
[Parent] Waiting for SIGCHLD event... (sleeping 1s)
[SIGNAL HANDLER: SIGCHLD] Child with PID 11823 terminated with exit status 10.

[Parent] Child termination successfully handled via SIGCHLD!
=================================================="""
    },
    {
        "id": "Q7",
        "title": "File Attributes Inspection using stat() System Call",
        "filename": "7_lab_stat.c",
        "description": "Develop a C program that accepts one or more filenames as command-line arguments and uses the stat() system call to display the inode number, file type, file size, permissions, owner ID, group ID, hard-link count, and access/modification/status-change timestamps for each file.",
        "concepts": "struct stat, st_ino, S_ISREG/S_ISDIR macros, st_size, permission bits masking, st_uid, st_gid, st_nlink, atime/mtime/ctime timestamp formatting via strftime.",
        "sample_output": """=================================================================
 File Attributes for: 7_lab_stat.c
=================================================================
  Inode Number:            21635669
  File Type:               Regular File (-)
  File Size:               3682 bytes
  Permissions:             rw-r--r-- (octal 644)
  Owner User ID (UID):     1000
  Group ID (GID):          1000
  Hard-link Count:         1
  Last Access Time:        2026-10-11 21:53:56
  Last Modification Time:  2026-10-11 21:51:38
  Last Status Change Time: 2026-10-11 21:51:38
-----------------------------------------------------------------"""
    },
    {
        "id": "Q8",
        "title": "Hard Links and Symbolic Links Creation & Distinction",
        "filename": "8_lab_links.c",
        "description": "Develop a C program to create a hard link and a symbolic (soft) link to a specified original file using link() and symlink(). Display the inode number and link count of the original file, hard link, and symbolic link. Use stat() and lstat() appropriately to distinguish the symbolic link from its target.",
        "concepts": "Hard links (link()), Symbolic links (symlink()), inode sharing, link count incrementation, stat() dereferencing vs lstat() link inspection.",
        "sample_output": """========================================================================================
 Experiment 8: Hard Link and Symbolic Link Demonstration
========================================================================================

[Step 1] Created original file: 'orig_file.txt'
[Step 2] Created hard link 'hard_link.txt' -> 'orig_file.txt' using link()
[Step 3] Created symbolic link 'soft_link.txt' -> 'orig_file.txt' using symlink()

----------------------------------------------------------------------------------------
File Attributes Comparison:
----------------------------------------------------------------------------------------
Original File        | Path: orig_file.txt        | Inode: 21635710   | Links: 2    | Type: Regular File
Hard Link            | Path: hard_link.txt        | Inode: 21635710   | Links: 2    | Type: Regular File
Soft Link (lstat)    | Path: soft_link.txt        | Inode: 21635711   | Links: 1    | Type: Symbolic Link
Soft Link (stat)     | Path: soft_link.txt        | Inode: 21635710   | Links: 2    | Type: Regular File
----------------------------------------------------------------------------------------"""
    },
    {
        "id": "Q9",
        "title": "Sequential File I/O using POSIX System Calls",
        "filename": "9_lab_file_io.c",
        "description": "Develop a C program using the POSIX file-I/O system calls open(), write(), read(), and close() to create a file, write student/academic data into it, and subsequently read and display the contents sequentially. Handle file-opening, reading, and writing errors appropriately.",
        "concepts": "Low-level unbuffered POSIX file I/O, open(O_CREAT | O_WRONLY | O_TRUNC), write() byte transfer, read() sequential reading in buffers, close(), error validation.",
        "sample_output": """==================================================
 Experiment 9: Sequential File I/O using POSIX Calls
==================================================

[Step 1] Creating and opening 'student_records.txt' for writing...
[SUCCESS] File descriptor obtained: 10

[Step 2] Writing student academic records into file...
[SUCCESS] 218 bytes successfully written to file.

[SUCCESS] File closed successfully after writing.

[Step 3] Opening 'student_records.txt' in read-only mode to verify contents...
[Step 4] Sequentially reading file contents:
--------------------------------------------------------------------------
Roll No: 101 | Name: Sai Nikhil | Course: Operating Systems | Grade: A+
Roll No: 102 | Name: Alice Smith | Course: Computer Networks | Grade: A
Roll No: 103 | Name: Bob Johnson | Course: Database Systems   | Grade: B+
--------------------------------------------------------------------------
[SUCCESS] File closed successfully. Sequential I/O demonstration finished.
=================================================="""
    },
    {
        "id": "Q10",
        "title": "Random File Access using lseek() and Metadata via stat()",
        "filename": "10_lab_lseek.c",
        "description": "Extend the program to demonstrate random file access using lseek(). Move the file offset to a specified byte position and read data from that position. Use the stat() system call to display the file's inode number, file size, number of links, user ID, group ID, and file mode.",
        "concepts": "Random access file pointer positioning using lseek(), SEEK_SET (from beginning), SEEK_CUR (relative jump), SEEK_END (from EOF), stat() inspection.",
        "sample_output": """==================================================
 Experiment 10: Random File Access (lseek) & stat
==================================================

[Setup] Created 'random_access_demo.txt' with 4 structured records (20 bytes each).

--------------------------------------------------
 File Metadata using stat(): 'random_access_demo.txt'
--------------------------------------------------
  Inode Number:        21635717
  File Size:           80 bytes
  Number of Links:     1
  User ID (UID):       1000
  Group ID (GID):      1000
  File Mode:           0644 (octal permissions)
--------------------------------------------------

[lseek] Repositioned offset to 40 bytes from start (SEEK_SET).
[Read at offset 40]: "Record 2: Item-CCCC"

[lseek] Advanced offset by 1 byte from current position (SEEK_CUR) -> Now at 60.
[Read at offset 60]: "Record 3: Item-DDDD"

[lseek] Repositioned to -20 bytes from EOF (SEEK_END) -> Now at 60.
[Read at offset 60]: "Record 3: Item-DDDD"

[Clean-up] File closed. Random access demonstration finished.
=================================================="""
    },
    {
        "id": "Q11",
        "title": "Multithreaded Array Sum using POSIX Threads (pthread)",
        "filename": "11_lab_pthread_sum.c",
        "description": "Develop a multithreaded C program using POSIX threads (pthread) to calculate the sum of a large integer array. Divide the array into approximately equal portions and assign each portion to a separate thread. Each thread must calculate and display its local sum.",
        "concepts": "POSIX Threads (pthread), pthread_create(), pthread_join(), chunk partitioning, thread arguments struct, parallel local sums and global aggregation.",
        "sample_output": """==================================================
 Experiment 11: Multithreaded Array Sum (pthreads)
==================================================

Allocating and initializing array of 1000000 elements...
Creating 4 worker threads to compute partial sums...

[Thread 0] Processed index [      0 to  249999] | Local Sum = 250000
[Thread 3] Processed index [ 750000 to  999999] | Local Sum = 250000
[Thread 1] Processed index [ 250000 to  499999] | Local Sum = 250000
[Thread 2] Processed index [ 500000 to  749999] | Local Sum = 250000

--------------------------------------------------
All threads completed execution.
Total Sum of 1000000 array elements: 1000000
Verification: Expected Sum = 1000000 | Status: PASSED
=================================================="""
    },
    {
        "id": "Q12",
        "title": "Concurrent Multiprocess & Multithreaded Log Analyzer",
        "filename": "12_lab_log_analyzer.c",
        "description": "Develop a concurrent Linux program to analyze a log file containing INFO, WARNING, and ERROR messages. Use fork() to divide the workload between a parent and child process. Within each process, create two POSIX threads using pthread_create()—one to count INFO messages and another to count ERROR messages—while the main flow counts WARNING messages.",
        "concepts": "Hybrid concurrency combining multiprocessing (fork()) and multithreading (pthread_create()), IPC pipe for worker result aggregation.",
        "sample_output": """=================================================================
 Experiment 12: Concurrent Log File Analysis (fork + pthreads)
=================================================================

Total log lines in file 'system_log.txt': 12 lines

[Parent PID 11927] Processed lines 1 to 6 (Count = 6 lines)
[Child PID 11933] Processed lines 7 to 12 (Count = 6 lines)
  -> Parent INFO count:    3
  -> Child INFO count:    2
  -> Parent ERROR count:   1
  -> Child ERROR count:   2
  -> Parent WARNING count: 2
  -> Child WARNING count: 2

-----------------------------------------------------------------
 FINAL CONSOLIDATED LOG ANALYSIS REPORT:
-----------------------------------------------------------------
 Total INFO messages:    5 (Parent: 3, Child: 2)
 Total WARNING messages: 4 (Parent: 2, Child: 2)
 Total ERROR messages:   3 (Parent: 1, Child: 2)
 Total analyzed entries: 12
================================================================="""
    }
]

def add_code_box(doc, code_text):
    tbl = doc.add_table(rows=1, cols=1)
    tbl.alignment = WD_TABLE_ALIGNMENT.CENTER
    tbl.autofit = False
    cell = tbl.cell(0, 0)
    cell.width = Inches(6.8)
    
    # Set background shading
    tcPr = cell._tc.get_or_add_tcPr()
    shd = parse_xml(f'<w:shd {nsdecls("w")} w:fill="{CODE_BG}"/>')
    tcPr.append(shd)
    
    # Border styling (light gray border)
    borders = parse_xml(
        f'<w:tcBorders {nsdecls("w")}>'
        f'  <w:top w:val="single" w:sz="4" w:space="0" w:color="D5DBDB"/>'
        f'  <w:left w:val="single" w:sz="18" w:space="0" w:color="2980B9"/>'
        f'  <w:bottom w:val="single" w:sz="4" w:space="0" w:color="D5DBDB"/>'
        f'  <w:right w:val="single" w:sz="4" w:space="0" w:color="D5DBDB"/>'
        f'</w:tcBorders>'
    )
    tcPr.append(borders)
    
    p = cell.paragraphs[0]
    p.paragraph_format.space_before = Pt(4)
    p.paragraph_format.space_after = Pt(4)
    p.paragraph_format.line_spacing = Pt(11)
    
    run = p.add_run(code_text)
    run.font.name = "Consolas"
    run.font.size = Pt(8.5)
    run.font.color.rgb = RGBColor(30, 30, 30)

def add_output_box(doc, output_text):
    tbl = doc.add_table(rows=1, cols=1)
    tbl.alignment = WD_TABLE_ALIGNMENT.CENTER
    tbl.autofit = False
    cell = tbl.cell(0, 0)
    cell.width = Inches(6.8)
    
    tcPr = cell._tc.get_or_add_tcPr()
    shd = parse_xml(f'<w:shd {nsdecls("w")} w:fill="{OUTPUT_BG}"/>')
    tcPr.append(shd)
    
    borders = parse_xml(
        f'<w:tcBorders {nsdecls("w")}>'
        f'  <w:top w:val="none"/>'
        f'  <w:left w:val="single" w:sz="18" w:space="0" w:color="27AE60"/>'
        f'  <w:bottom w:val="none"/>'
        f'  <w:right w:val="none"/>'
        f'</w:tcBorders>'
    )
    tcPr.append(borders)
    
    p = cell.paragraphs[0]
    p.paragraph_format.space_before = Pt(4)
    p.paragraph_format.space_after = Pt(4)
    p.paragraph_format.line_spacing = Pt(11)
    
    run = p.add_run(output_text)
    run.font.name = "Consolas"
    run.font.size = Pt(8.5)
    run.font.color.rgb = RGBColor(236, 240, 241) # Off white

for exp in experiments:
    # Heading
    h = doc.add_heading(level=1)
    hrun = h.add_run(f"Experiment {exp['id']}: {exp['title']}")
    hrun.font.name = "Arial"
    hrun.font.size = Pt(14)
    hrun.font.color.rgb = PRIMARY_COLOR
    hrun.bold = True
    
    # Metadata info table
    info_table = doc.add_table(rows=3, cols=2)
    info_table.autofit = False
    info_table.alignment = WD_TABLE_ALIGNMENT.CENTER
    col_widths = [Inches(1.5), Inches(5.3)]
    
    labels = ["Problem Statement", "Source File Name", "Key Concepts & Calls"]
    vals = [exp['description'], exp['filename'], exp['concepts']]
    
    for row_idx, (lbl, val) in enumerate(zip(labels, vals)):
        row = info_table.rows[row_idx]
        
        # Label cell
        c0 = row.cells[0]
        c0.width = col_widths[0]
        p0 = c0.paragraphs[0]
        p0.paragraph_format.space_before = Pt(2)
        p0.paragraph_format.space_after = Pt(2)
        r0 = p0.add_run(lbl)
        r0.bold = True
        r0.font.name = "Arial"
        r0.font.size = Pt(9)
        r0.font.color.rgb = PRIMARY_COLOR
        
        # Value cell
        c1 = row.cells[1]
        c1.width = col_widths[1]
        p1 = c1.paragraphs[0]
        p1.paragraph_format.space_before = Pt(2)
        p1.paragraph_format.space_after = Pt(2)
        r1 = p1.add_run(val)
        r1.font.name = "Arial"
        r1.font.size = Pt(9)
        
        # Subtle light styling
        shd0 = parse_xml(f'<w:shd {nsdecls("w")} w:fill="EBF5FB"/>')
        c0._tc.get_or_add_tcPr().append(shd0)
    
    doc.add_paragraph() # Spacing
    
    # C Source Code section
    sub1 = doc.add_heading(level=2)
    r_sub1 = sub1.add_run(f"C Source Code ({exp['filename']}):")
    r_sub1.font.name = "Arial"
    r_sub1.font.size = Pt(11)
    r_sub1.font.color.rgb = SECONDARY_COLOR
    r_sub1.bold = True
    
    with open(f"/home/sai-nikhil/os_lab/{exp['filename']}", "r") as f:
        code_content = f.read()
    
    add_code_box(doc, code_content)
    
    # Execution Output section
    sub2 = doc.add_heading(level=2)
    r_sub2 = sub2.add_run("Sample Execution Output:")
    r_sub2.font.name = "Arial"
    r_sub2.font.size = Pt(11)
    r_sub2.font.color.rgb = SECONDARY_COLOR
    r_sub2.bold = True
    
    add_output_box(doc, exp['sample_output'])
    
    # Page break after each experiment
    doc.add_page_break()

output_docx_path = "/home/sai-nikhil/os_lab/OS_Lab_Programs.docx"
doc.save(output_docx_path)
print(f"Successfully generated {output_docx_path}")
