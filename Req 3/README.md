# 📘 SubTool – Subtitle File Processing Utility

# 1. Problem Statement

The objective of this assignment is to implement a command-line utility in C named `sub_tool` that processes subtitle files (.srt).

The tool supports viewing, editing, inserting, and extracting specific parts of a file.

The program must:

- Accept command-line arguments
- Handle file I/O safely
- Perform line-based operations
- Handle errors precisely
- Compile using a Makefile

---

# 2. Functional Requirements

The tool supports the following flags:

### Viewing Operations

| Flag | Description |
| --- | --- |
| `-v` | View entire file |
| `-v -n <line>` | View specific line |
| `-f <n>` | View first n lines |
| `-l <n>` | View last n lines |

### Editing Operations

---

# 3. Program Architecture

The project is divided into:

```
requirement/
│
├── main.c
├── sub_tool.c
├── sub_tool.h
└── Makefile
```

### Separation of Concerns

- `main.c` → argument parsing & control flow
- `sub_tool.c` → file manipulation logic
- `sub_tool.h` → function declarations
- `Makefile` → compilation automation

This modular design improves maintainability and readability.

---

# 4. Core C Concepts Used

## 4.1 File Handling

We used:

```c
FILE*fopen()
fgets()
fputs()
fclose()
remove()
rename()
```

### fopen()

Opens a file and returns a FILE pointer.

```c
FILE*fp=fopen(filename,"r");
```

If file does not exist:

```c
if (fp==NULL)
```

---

### fgets()

Reads a line safely into a buffer.

```c
fgets(buffer,MAX_LINE_LENGTH,fp);
```

Prevents buffer overflow.

---

### fputs()

Writes a string to a file.

```c
fputs(buffer,fp);
```

Used for writing modified content.

---

### remove()

Deletes a file from disk.

```c
remove("temp.txt");
```

---

### rename()

Renames a file.

```c
rename("temp.txt","original.txt");
```

Used when overwriting original file after editing.

---

# 5. Editing Strategy (Important Design Choice)

When editing or inserting:

We never modify the file directly.

Instead:

1. Open original file for reading
2. Open temporary file for writing
3. Copy content line by line
4. Apply modification when needed
5. Close both files
6. Replace original file using:
    - remove()
    - rename()

This approach prevents file corruption.

---

# 6. Error Handling Strategy

We implemented:

### File Not Found

```c
fprintf(stderr,"Error: File does not exist.\n");
exit(0);
```

### Line Out of Bounds

```c
fprintf(stderr,"Error: Line out of bounds.\n");
exit(0);
```

### Requested Lines Exceed File Length

```c
fprintf(stderr,"Error: File limit exceeded.\n");
exit(0);
```

Important:

- Errors printed to stderr
- Program exits with status 0 → AUTO GRADING ISSUE
- Exact string formatting

---

# 7. Counting Lines

To validate bounds, we implemented:

```c
int count_lines(const char* filename)
```

Logic:

- Open file
- Count number of lines using fgets
- Return total

Used before performing:

- `n`
- `f`
- `l`

---

# 8. Makefile

The Makefile automates compilation:

```
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: sub_tool

sub_tool: main.o sub_tool.o
	$(CC) $(CFLAGS) -o sub_tool main.o sub_tool.o

main.o: main.c sub_tool.h
	$(CC) $(CFLAGS) -c main.c

sub_tool.o: sub_tool.c sub_tool.h
	$(CC) $(CFLAGS) -c sub_tool.c

clean:
	rm -f sub_tool main.o sub_tool.o
```

Commands:

```
make
make clean
```

---

# 9. Testing

Tested using:

- Manual test cases
- Autograder (GitHub Actions)
- Edge cases:
    - Non-existent file
    - Invalid line number
    - Excess lines requested

---
