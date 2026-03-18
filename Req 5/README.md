# File Search Using Processes and Signals

## Overview

This program demonstrates **process creation, inter-process communication using signals, and file system inspection** in C on a Unix/Linux system.

The program receives:

1. A **target file size**
2. A **list of file names**

Two child processes are created. Each child searches **half of the files** for a file whose size matches the target size.

The **first child that finds the file becomes the winner** and terminates with a special exit code. The parent detects the winner and signals the other child to terminate as the loser.

If **no child finds the file within 5 seconds**, the parent sends a timeout signal and both children terminate.

This assignment demonstrates:

- `fork()` for process creation
- UNIX **signals** for communication
- `stat()` for file metadata
- `wait()` for process synchronization
- **exit status inspection** using `WEXITSTATUS()`

---

# Program Architecture

The program consists of **three processes**:

```
           Parent Process
           /            \
     Child A          Child B
```

Responsibilities:

| Process | Responsibility |
| --- | --- |
| Parent | Creates children, waits for results, determines winner |
| Child A | Searches first half of files |
| Child B | Searches second half of files |

---

# Header Files Used

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
```

Purpose of each header:

| Header | Purpose |
| --- | --- |
| `stdio.h` | Input/output functions (`printf`) |
| `stdlib.h` | Utility functions (`atoi`, `exit`) |
| `unistd.h` | UNIX system calls (`fork`, `pause`) |
| `sys/types.h` | Data types used by system calls |
| `sys/stat.h` | File metadata retrieval (`stat`) |
| `signal.h` | Signal handling |
| `sys/wait.h` | Process synchronization (`wait`) |

---

# Global Variables

```c
pid_t pidA,pidB;
```

These store the **process IDs of the two child processes**, allowing the parent to send signals to them.

---

# Signal Handlers

The program defines **three signal handlers**.

---

## 1. Loser Handler

```c
void loser_handler(int sig)
```

Triggered when a child receives:

```c
SIGUSR1
```

Meaning:

> The other child already found the file.
> 

Action:

```
Print loser message
Terminate process
```

Output:

```
I am the child and I received from my parent that I am the loser.
```

---

## 2. File Not Found Handler

```c
void notfound_handler(int sig)
```

Triggered when:

```
SIGUSR2
```

Meaning:

> Search timed out and no child found the file.
> 

Output:

```
I am the child and I could not find the file.
```

The process then terminates.

---

## 3. Alarm Handler (Parent)

```c
void alarm_handler(int sig)
```

Triggered when the parent alarm expires.

The parent sends:

```
SIGUSR2 → Child A
SIGUSR2 → Child B
```

This causes both children to execute `notfound_handler`.

---

# Input Format

Program execution format:

```
./program target_size file1 file2 file3 ...
```

Example:

```
./lab4 150 file1.txt file2.txt file3.txt file4.txt
```

Parameters:

| Argument | Description |
| --- | --- |
| `argv[1]` | Target file size |
| `argv[2...]` | List of files to search |

---

# File Distribution Between Children

The list of files is split between the children.

```
Child A → first half
Child B → second half
```

If the number of files is **odd**:

```
Child A gets one extra file
```

Implementation:

```c
if (num_files % 2 == 0)
```

Example:

```
Files: 5

Child A → 3 files
Child B → 2 files
```

---

# Child Search Algorithm

Each child performs the following steps:

```
for each assigned file
    call stat()
    read file size
    compare with target size
```

If a match is found:

```
print file location
exit(1)
```

Example output:

```
I found the file at location 3.
```

Exit code **1** indicates the **winner**.

---

# Why Exit Code 1 is Used

The parent identifies the winner using:

```c
WIFEXITED(status)
WEXITSTATUS(status)
```

Meaning:

| Exit Code | Meaning |
| --- | --- |
| `1` | Child found the file |
| `0` | Normal termination (loser or timeout) |

This ensures the parent **only declares a winner if a file was actually found**.

---

# Parent Process Logic

After creating both children, the parent:

### 1. Starts a timeout

```c
alarm(5)
```

This means:

```
Search must finish within 5 seconds
```

---

### 2. Waits for the first child to terminate

```c
pid_t finished = wait(&status);
```

This returns:

```
PID of the child that finished first
```

---

### 3. Cancels the alarm

```c
alarm(0)
```

Because a result was obtained.

---

### 4. Checks exit status

```c
if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
```

Meaning:

> A child successfully found the file.
> 

---

### 5. Declares the winner

If `finished == pidA`

```
Parent: Child A found the file.
```

Otherwise:

```
Parent: Child B found the file.
```

---

### 6. Terminates the loser

The parent sends:

```c
SIGUSR1
```

to the other child.

This triggers `loser_handler`.

---

### 7. Cleans up remaining child

```c
wait(NULL);
```

This prevents **zombie processes**.

---

# Process Timeline

Example scenario when **Child B finds the file first**:

```
Parent creates Child A
Parent creates Child B

Child A searches files
Child B searches files

Child B finds file
Child B exits with code 1

Parent wakes from wait()
Parent prints winner message

Parent sends SIGUSR1 to Child A
Child A prints loser message
Child A exits

Parent waits for final child
Program ends
```

---

# Timeout Scenario

If **no file matches the target size**:

```
5 seconds pass
alarm triggers
```

Parent sends:

```
SIGUSR2 to both children
```

Children print:

```
I am the child and I could not find the file.
```

Both processes terminate.

The parent then finishes execution.

---

# Compilation

Compile using:

```
gcc fileSearch.c -o fileSearch
```

---

# Example Execution

### Command

```
./lab4 200 file1.txt file2.txt file3.txt file4.txt
```

### Possible Output

```
I found the file at location 2.
Parent: Child B found the file.
I am the child and I received from my parent that I am the loser.
```

---

# Key Concepts Demonstrated

This program demonstrates several **Operating Systems concepts**:

| Concept | Description |
| --- | --- |
| Process Creation | `fork()` |
| Interprocess Communication | Signals |
| Synchronization | `wait()` |
| File Metadata Access | `stat()` |
| Signal Handling | `signal()` |
| Timeout Control | `alarm()` |
| Exit Status Inspection | `WEXITSTATUS()` |
