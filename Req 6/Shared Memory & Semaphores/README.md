# Shared Memory & Semaphores Client–Server System

## Overview

This program demonstrates **inter-process communication (IPC)** using:

- **Shared Memory**
- **System V Semaphores**

The system consists of:

- A **client process** that sends requests
- A **server process** that processes the request and returns results

The client can request:

1. **Word count**
2. **Vowel count**

The client writes data into shared memory, signals the server, and waits for the response.

# System Architecture

The system uses **shared memory + 3 semaphores**:

```
        Client Process
              |
       (writes request)
              ↓
      Shared Memory Segment
              ↑
       (writes result)
              |
        Server Process
```

# Key Concepts

| Concept                 | Description                             |
| ----------------------- | --------------------------------------- |
| Shared Memory           | Fast IPC using `shmget`, `shmat`        |
| Semaphores              | Synchronization using `semget`, `semop` |
| Mutual Exclusion        | Prevent race conditions                 |
| Process Synchronization | Request/response coordination           |
| Multi-client Handling   | Safe concurrent access                  |
| Signal Handling         | Resource cleanup                        |

# Shared Memory Structure

```c
struct shared_data {
int client_id;
int choice;
char text[256];
int result;
};
```

### Fields

| Field       | Description           |
| ----------- | --------------------- |
| `client_id` | PID of client         |
| `choice`    | 1 = Words, 2 = Vowels |
| `text`      | Input sentence        |
| `result`    | Computed result       |

# Semaphore Design

We use **3 semaphores**:

| Index | Name     | Purpose                |
| ----- | -------- | ---------------------- |
| 0     | Mutex    | Protect shared memory  |
| 1     | Request  | Client → Server signal |
| 2     | Response | Server → Client signal |

# Communication Flow

```
Client:
    Lock mutex
    Write data to shared memory
    Signal request semaphore

Server:
    Wait for request
    Process data
    Write result
    Signal response

Client:
    Wait for response
    Read result
    Unlock mutex
```
