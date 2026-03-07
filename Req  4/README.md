# 📘Concurrent File Size Search Utility

# 1.Problem Statement

The objective of this assignment is to implement a **concurrent command-line utility in C** that searches for a file of a specific size among a list of files.

The program uses **multiple processes and UNIX signals** to perform the search concurrently.

The program must:

- Accept command-line arguments.
- Divide the workload between **two child processes**.
- Use **stat()** to read file metadata.
- Use **signals** for process communication.
- Implement a **timeout mechanism** using `alarm()`.
- Handle process termination safely.
- Avoid zombie processes.

The search terminates when:

- One child finds the file, or
- No file is found within **5 seconds**.

---

# 2.Functional Requirements

The program is executed as:

```
./lab4 <target_size> <file1> <file2> <file3> ...
```

### Example

```
./lab4 400 f1.txt f2.txt f3.txt f4.txt
```

Where:

| Argument | Description |
| --- | --- |
| `target_size` | File size to search for (bytes) |
| `file1..fileN` | Files to check |

---

## Expected Behavior

### Case 1 – File Found

If a child finds a file with the requested size:

```
I found the file at location X.
Parent: Child A/B found the file.
I am the child and I received from my parent that I am the loser.
```

Where **X is the index of the file** in the provided list (0-based).

---

### Case 2 – File Not Found (Timeout)

If no child finds the file within **5 seconds**:

```
I am the child and I could not find the file.
I am the child and I could not find the file.
```

The parent **does not print anything** in this case.

---

# 3.Program Architecture

The program consists of **three processes**:

```
            Parent Process
             /        \
       Child A      Child B
```

### Responsibilities

| Process | Responsibility |
| --- | --- |
| Parent | Manage children and coordinate signals |
| Child A | Search first half of files |
| Child B | Search second half of files |

---

# 4.Core Operating System Concepts Used

---

# 4.1 Process Creation – `fork()`

The program creates two child processes using:

```c
pid_t pidA=fork();
pid_t pidB=fork();
```

Each `fork()` creates a **duplicate process**.

Return values:

| Return Value | Meaning |
| --- | --- |
| `0` | Child process |
| `>0` | Parent process |
| `<0` | Fork failed |

---

# 4.2 File Metadata Retrieval – `stat()`

Instead of opening files, we retrieve file metadata using:

```c
struct stat info;
stat(filename,&info);
```

Important field:

```c
info.st_size
```

This gives the **file size in bytes**.

This approach is more efficient than reading file contents.

---

# 4.3 Workload Distribution

The parent divides the files into two groups.

Let:

```c
num_files = argc - 2
```

Then:

```
Child A → first half
Child B → second half
```

If the number of files is **odd**:

```
Child A receives the extra file
```

Example:

```
Files: 5

Child A → 3 files
Child B → 2 files
```

---

# 4.4 Signal-Based Interprocess Communication

Signals are used for communication between processes.

| Signal | Sender | Purpose |
| --- | --- | --- |
| SIGCHLD | OS | Notify parent a child terminated |
| SIGUSR1 | Parent | Inform a child it lost |
| SIGUSR2 | Parent | Inform children search failed |
| SIGALRM | OS | Trigger timeout |

---

# 4.5 Signal Handlers

Signal handlers are used to define custom responses.

Example:

```c
void loser_handler(int sig){
printf("I am the child and I received from my parent that I am the loser.\n");
exit(0);
}
```

Handlers allow the program to react to events asynchronously.

---

# 4.6 Timeout Mechanism – `alarm()`

The parent process sets a timeout:

```c
alarm(5);
```

Meaning:

```
Send SIGALRM to this process after 5 seconds
```

The handler then terminates both children.

```c
kill(pidA,SIGUSR2);
kill(pidB,SIGUSR2);
```

---

# 5.Search Algorithm

### Child Process Logic

```
for each assigned file
    call stat()
    compare st_size with target_size
    if match
        print location
        exit()
```

If no file matches:

```c
pause()
```

The child waits for a signal from the parent.

---

# 6.Parent Coordination Logic

Parent algorithm:

```
1. Create child A
2. Create child B
3. Install SIGALRM handler
4. Start 5-second timer
5. Wait for child termination
6. If timeout occurred
       do nothing
   else
       print winner
       notify loser
7. Wait for remaining child
```

---

# 7.Critical Design Issues Encountered

During development, several concurrency issues were discovered.

---

# 7.1 Misinterpreting Child Exit

Initial implementation assumed:

```
child exit → file found
```

However, children may also exit due to **timeout signals**.

This caused incorrect output:

```
I am the child and I could not find the file.
I am the child and I could not find the file.
Parent: Child A found the file.
```

### Solution

A **timeout flag** was introduced.

```c
int timeout_occured = 0;
```

The parent only prints the winner if:

```c
timeout_occured == 0
```

---

# 7.2 Zombie Processes

If a child terminates and the parent does not call `wait()`:

```
Zombie process created
```

To prevent this:

```c
wait(NULL);
```

Both children are collected before program exit.

---

# 8.System Calls and Functions Used

| Function | Purpose |
| --- | --- |
| `fork()` | Create child process |
| `stat()` | Retrieve file metadata |
| `signal()` | Register signal handlers |
| `kill()` | Send signals |
| `alarm()` | Schedule timeout |
| `pause()` | Block process until signal |
| `wait()` | Collect child termination |
| `exit()` | Terminate process |
