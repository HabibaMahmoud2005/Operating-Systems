# Shared Memory & Semaphores (Client–Server IPC)

## Overview

This program demonstrates **inter-process communication (IPC)** using:

- **Shared Memory** → for data exchange
- **Semaphores** → for synchronization

The system follows a **client–server architecture**:

- The **client** sends a request (text + operation)
- The **server** processes the request
- The **server writes the result back**
- The **client reads the result**

## Program Architecture

```
        Client Process
              ↓
      [Shared Memory]
              ↑
        Server Process
```

### Responsibilities

| Process | Responsibility |
| --- | --- |
| Client | Takes user input and sends request |
| Server | Processes request and returns result |

## Data Structure

The shared memory uses a structured format:

```c
struct shared_data {
int client_id;
int choice;// 1 = words, 2 = vowels
char text[256];
int result;
};
```

### Field Description

| Field | Description |
| --- | --- |
| `client_id` | Unique ID (last 4 digits of PID) |
| `choice` | Operation requested |
| `text` | Input sentence |
| `result` | Computed result |

## Keys and IPC Objects

Both client and server use:

```c
ftok("keyFile",65);  // shared memory
ftok("keyFile",75);  // semaphores
```

This ensures **both processes access the same IPC resources**.

## Semaphores Design

Two semaphores are used:

| Semaphore | Purpose |
| --- | --- |
| `sem[0]` | Client → Server notification |
| `sem[1]` | Server → Client notification |

## Synchronization Flow

### Step-by-step execution:

### 1. Client writes data

```c
data->client_id = getpid()%10000;
data->choice = choice;
strcpy(data->text,sentence);
```

### 2. Client signals server

```c
sem_op(sem_id,0,1);
```

Meaning:

> "Server, I placed a request"
> 

### 3. Server waits for request

```c
sem_op(sem_id,0,-1);
```

Meaning:

> Server blocks until client sends data
> 

### 4. Server processes request

Depending on `choice`:

- `1` → count words
- `2` → count vowels

### 5. Server writes result

```c
data->result = result;
```

### 6. Server signals client

```c
sem_op(sem_id,1,1);
```

Meaning:

> "Client, your result is ready"
> 

### 7. Client waits for result

```c
sem_op(sem_id,1,-1);
```

### 8. Client reads result

```c
printf("Result = %d\n",data->result);
```

## Semaphore Operation Function

Helper function:

```c
void sem_op(int sem_id,int sem_num,int op_val)
```

### Parameters

| Parameter | Meaning |
| --- | --- |
| `sem_id` | Semaphore set ID |
| `sem_num` | Which semaphore (0 or 1) |
| `op_val` | Operation (+1 or -1) |

## Server Initialization

The server initializes semaphores:

```c
union Semun semun;
semun.val=0;

semctl(sem_id,0,SETVAL,semun);
semctl(sem_id,1,SETVAL,semun);
```

### Why?

To ensure:

```
Initial state = blocked
```

So processes synchronize correctly.

## Processing Logic

### Count Vowels

```c
int count_vowels(const char*str)
```

Counts:

```
a, e, i, o, u (case-insensitive)
```

### Count Words

```c
int count_words(const char*str)
```

Logic:

- Count transitions from space → non-space
- Tracks `in_word` state

## Example Execution

### Client Input

```
Choice: 2
Sentence: hello world
```

### Server Processing

```
Vowels = 3
```

### Output

```
Result = 3
```

## Compilation

```bash
gcc server.c -o server
gcc client.c -o client
```

---

## Execution

Run server first:

```bash
./server
```

Then client:

```bash
./client
```

## Key Concepts Demonstrated

| Concept | Description |
| --- | --- |
| Shared Memory | Fast IPC mechanism |
| Semaphores | Process synchronization |
| Client–Server Model | Task distribution |
| Blocking Mechanism | Controlled execution order |
| IPC Keys (`ftok`) | Resource identification |

## Important Notes

- Server must start **before client**
- Both must use the **same key file**
- Semaphore initialization should be done **only once (by server)**
- Missing synchronization leads to:
    - Race conditions
    - Incorrect results
