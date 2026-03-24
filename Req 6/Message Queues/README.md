# Message Queue Based Client–Server System

## Overview

This program demonstrates **inter-process communication (IPC)** using **System V Message Queues** in C on a Unix/Linux system.

The system consists of:

- A **client process** that sends requests
- A **server process** that processes requests and returns results

The client can request one of two services:

1. **Word count**
2. **Vowel count**

The server receives the request, processes it, and sends the result back to the correct client.

# System Architecture

The system uses **two message queues**:

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

# Key Concepts

| Concept                  | Description                               |
| ------------------------ | ----------------------------------------- |
| Message Queues           | IPC using `msgget`, `msgsnd`, `msgrcv`    |
| Client–Server Model      | Separation between request and processing |
| Message Types            | Used to route responses to correct client |
| Blocking vs Non-blocking | Controlled using flags                    |
| Multi-client Handling    | Multiple clients supported simultaneously |
| Signal Handling          | Cleanup of queues on termination          |

# Queue Design

Two queues are used:

| Queue        | Purpose                      |
| ------------ | ---------------------------- |
| Words Queue  | Handles word count requests  |
| Vowels Queue | Handles vowel count requests |

## Queue Keys

```c
ftok("keyFile",65);// Words Queue
ftok("keyFile",75);// Vowels Queue
```

# Communication Flow

### Example: Word Count Request

```
Client:
    Enter sentence
    Choose WORDS

Client → Words Queue:
    Send message (type = client_id)

Server:
    Receives message
    Processes it
    Sends result back (same type)

Client:
    Receives result
    Prints output
```
