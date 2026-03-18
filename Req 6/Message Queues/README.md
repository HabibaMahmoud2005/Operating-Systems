# Message Queue Based Client–Server System

## Overview

This program demonstrates **inter-process communication (IPC)** using **System V Message Queues** in C on a Unix/Linux system.

The system consists of:

- A **client process** that sends requests
- A **server process** that processes requests and returns results

The client can request one of two services:

1. **Word count**
2. **Vowel count**

The server receives the request, performs the computation, and sends the result back to the correct client.

# System Architecture

The system follows a **Client–Server model** using **two message queues**:

```
        Client Process
             |
     ---------------------
     |                   |
 Words Queue      Vowels Queue
     |                   |
     ---------------------
             |
        Server Process
```

# Key Concepts Demonstrated

| Concept | Description |
| --- | --- |
| Message Queues | IPC using `msgget`, `msgsnd`, `msgrcv` |
| Client–Server Model | Separation of request and processing |
| Process Identification | Using PID as message type |
| Synchronization | Blocking receive operations |
| Signal Handling | Cleanup using `SIGINT` |

# Header Files Used

```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
```

### Purpose of Each Header

| Header | Purpose |
| --- | --- |
| `sys/types.h` | System data types |
| `sys/ipc.h` | IPC mechanisms |
| `sys/msg.h` | Message queue functions |
| `string.h` | String operations |
| `stdlib.h` | Memory and exit functions |
| `stdio.h` | Input/output |
| `unistd.h` | Process utilities |
| `signal.h` | Signal handling |

# Message Structure

```c
struct msgbuf {
long mtype;
char mtext[256];
};
```

### Fields

| Field | Description |
| --- | --- |
| `mtype` | Message type (used as client ID) |
| `mtext` | Message content (sentence or result) |

# Queue Design

Two queues are used:

| Queue | Purpose |
| --- | --- |
| Words Queue | Handles word count requests |
| Vowels Queue | Handles vowel count requests |

Each queue is identified using:

```c
ftok("keyFile",65);  // Words queue
ftok("keyFile",75);  // Vowels queue
```

# Client Process

## Responsibilities

1. Takes user input:
    - Service type (words or vowels)
    - Sentence
2. Sends request to server
3. Waits for result
4. Displays output

## Client Workflow

### 1. Create / Access Queues

```c
msgget(key,IPC_CREAT|0666);
```

### 2. Assign Client ID

```c
int client_id = getpid() % 10000;
buf.mtype = client_id;
```

This ensures:

- Each client receives only its own response

### 3. Send Request

```c
msgsnd(queue_id,&buf,sizeof(buf.mtext),!IPC_NOWAIT);
```

### 4. Receive Result

```c
msgrcv(queue_id,&buf,sizeof(buf.mtext),client_id,!IPC_NOWAIT);
```

### 5. Display Result

```c
printf("Result: %s\n",buf.mtext);
```

# Server Process

## Responsibilities

- Continuously listens for requests
- Processes data
- Sends results back to the correct client

## Server Workflow

### 1. Create Queues

```c
msgget(key,IPC_CREAT|0666);
```

### 2. Infinite Loop

```c
while(1)
```

The server continuously serves clients.

### 3. Receive Requests

```c
msgrcv(queue_id,&buf,sizeof(buf.mtext),0,IPC_NOWAIT);
```

- `0` → receive any message
- `IPC_NOWAIT` → non-blocking

### 4. Process Data

### Word Count

```c
int result = count_words(buf.mtext);
```

### Vowel Count

```c
int result = count_vowels(buf.mtext);
```

### 5. Convert Result to String

```c
sprintf(buf.mtext,"%d",result);
```

### 6. Send Response

```c
buf.mtype = client_id;
msgsnd(queue_id,&buf,sizeof(buf.mtext),!IPC_NOWAIT);
```

# Helper Functions

## Word Count Function

```c
int count_words(const char*str)
```

Logic:

```
Loop through string
Detect transitions from space → character
Count words
```

## Vowel Count Function

```c
int count_vowels(const char*str)
```

Checks for:

```
a, e, i, o, u (both lowercase and uppercase)
```

# Signal Handling

## Purpose

To clean up message queues when the server is terminated.

## Signal Used

```c
SIGINT // Triggered by Ctrl + C
```

## Handler Implementation

```c
void SIG_HANDLER(int signum)
{
   msgctl(WordsQueue_id,IPC_RMID,NULL);
   msgctl(VowelsQueue_id,IPC_RMID,NULL);
   exit(0);
}
```

## Registration

```c
signal(SIGINT,SIG_HANDLER);
```

# Communication Flow

### Example: Word Count Request

```
Client:
    Enter sentence
    Choose "Words"

Client → Words Queue:
    Send message (type = client_id)

Server:
    Receives message
    Computes word count
    Sends result back with same type

Client:
    Receives result
    Prints output
```

# Example Execution

### Input

```
Choose service:
1. Count Words
2. Count Vowels
Enter choice: 1

Enter a sentence:
Hello world from OS lab
```

### Output

```
Result: 5
```

# Error Handling

| Scenario | Handling |
| --- | --- |
| Queue creation failure | `perror()` + exit |
| Send failure | `perror()` + exit |
| Receive failure | `perror()` + exit |
| Invalid input | Program terminates |

# Compilation

```bash
gcc client.c-o client
gcc server.c-o server
```

# Execution

### Step 1: Run Server

```bash
./server
```

### Step 2: Run Client

```bash
./client
```
