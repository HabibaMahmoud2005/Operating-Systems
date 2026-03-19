# 📘 Multi-Process Order Processing System

## 1. Problem Statement

The objective of this assignment is to implement a multi-process C program that simulates a food delivery platform.

The system consists of:
- One parent process (platform)
- N child processes (dispatchers)

Each dispatcher processes a subset of customer orders and determines how many orders qualify for free delivery based on a minimum order value `V`.

The program must:
- Accept command-line arguments
- Read structured data from a file
- Use `fork()` to create child processes
- Use `exit()` and `wait()` for inter-process communication
- Print results in strict format
- Handle errors safely

---

## 2. Functional Requirements

### Command-Line Format

```bash
./program <input_file> <N> <V>
```

| Argument | Description |
|----------|------------|
| `input_file` | Path to input file |
| `N` | Number of dispatchers (child processes) |
| `V` | Minimum value for free delivery |

---

### Input File Format

```
M
items_1 price_1
items_2 price_2
...
items_M price_M
```

Where:
- `M` = total number of orders
- Each order contains:
  - Number of items
  - Price per item

Total order value is calculated as:

```
total = items × price
```

---

### Order Distribution Strategy

```
base_batch_size = M / N
remaining = M % N
```

- First `N-1` dispatchers process `base_batch_size` orders
- Last dispatcher processes `base_batch_size + remaining`

---

### Output Format

Program prints exactly one line:

```
x1 x2 x3 ... xN
```

Where:
- `xi` = number of orders processed by dispatcher `i`
- Only orders with total value ≥ `V` are counted
- No extra spaces
- No additional output

---

## 3. Core C Concepts Used

### File Handling
Functions used:
- `fopen()`
- `fscanf()`
- `fclose()`

### Dynamic Memory Allocation

```
int *total = malloc(orders * sizeof(int));
```

Memory released using:

```
free(total);
```

### Process Creation

```
int pid = fork();
```

Return values:
- `0` → Child process
- `> 0` → Parent process
- `-1` → Fork failed

### Inter-Process Communication

```
exit(count);
wait(&status);
WEXITSTATUS(status);
```

### Process Synchronization

```
wait(&status);
```

---

## 4. Dispatcher Logic

```
start = i * base_batch_size;

if(i == N - 1)
    end = start + base_batch_size + remaining;
else
    end = start + base_batch_size;
```

Each child:
- Iterates through assigned range
- Counts orders where `total[j] >= V`
- Returns result using `exit(count)`

---

## 5. Error Handling Strategy

Checks implemented:
- Invalid argument count
- File not found
- Fork failure

Errors printed using:
- `fprintf(stderr, ...)`
  
---

## 6. Key Operating System Concepts Demonstrated

- Process creation
- Process hierarchy
- Exit status propagation
- Zombie process handling
- PID-based process tracking
  
