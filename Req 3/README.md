## 1. 📌 Problem Statement

The objective of this assignment is to implement a command-line utility in C named `sub_tool` that processes subtitle files (.srt).

The tool supports viewing, editing, inserting, and extracting specific parts of a file.

The program must:

- Accept command-line arguments
- Handle file I/O safely
- Perform line-based operations
- Handle errors precisely
- Compile using a Makefile

---

# 2. ⚙ Functional Requirements

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

# 3. 🏗 Program Architecture

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

# 4. 🧠 Core C Concepts Used

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

# 5. 🔁 Editing Strategy (Important Design Choice)

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

# 6. ❗ Error Handling Strategy

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

# 7. 🧮 Counting Lines

To validate bounds, we implemented:

```c
int count_lines(constchar*filename)
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

# 8. 🛠 Makefile

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

# 9. 🧪 Testing

Tested using:

- Manual test cases
- Autograder (GitHub Actions)
- Edge cases:
    - Non-existent file
    - Invalid line number
    - Excess lines requested

---

# 10. 📌 Final Code

### main.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sub_tool.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Invalid usage.\n");
        return 1;
    }

    const char *input_file = argv[1];

//================= VIEW =================
    if (strcmp(argv[2], "-v") == 0)
    {
        if (argc == 5 && strcmp(argv[3], "-n") == 0)
        {
            int line = atoi(argv[4]);
            view_specific_line(input_file, line);
        }
        else if (argc == 3)
        {
            view_all(input_file);
        }
        else
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }
    }

//================= FIRST =================
    else if (strcmp(argv[2], "-f") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }

        int n = atoi(argv[3]);
        view_first_lines(input_file, n);
    }

//================= LAST =================
    else if (strcmp(argv[2], "-l") == 0)
    {
        if (argc != 4)
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }

        int n = atoi(argv[3]);
        view_last_lines(input_file, n);
    }

//================= INSERT =================
    else if (strcmp(argv[2], "-i") == 0)
    {
        const char *text = argv[3];
        const char *save_file = NULL;

        if (argc == 4)
        {
            insert_line(input_file, text, NULL);
        }
        else if (argc == 6 && strcmp(argv[4], "-s") == 0)
        {
            save_file = argv[5];
            insert_line(input_file, text, save_file);
        }
        else
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }
    }

//================= EDIT =================
    else if (strcmp(argv[2], "-e") == 0)
    {
        if (argc < 6)
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }

        const char *text = argv[3];

        if (strcmp(argv[4], "-n") != 0)
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }

        int line = atoi(argv[5]);
        const char *save_file = NULL;

        if (argc == 6)
        {
            edit_line(input_file, text, line, NULL);
        }
        else if (argc == 8 && strcmp(argv[6], "-s") == 0)
        {
            save_file = argv[7];
            edit_line(input_file, text, line, save_file);
        }
        else
        {
            fprintf(stderr, "Invalid usage.\n");
            return 1;
        }
    }

    else
    {
        fprintf(stderr, "Invalid flag.\n");
        return 1;
    }

    return 0;
}
```

### sub_tool.h

```c
#ifndef SUB_TOOL_H
#define SUB_TOOL_H

#include <stdio.h>
#define MAX_LINE_LENGTH 1024

void view_all(const char *filename);
void view_specific_line(const char *filename, int line);
void view_first_lines(const char *filename, int n);
void view_last_lines(const char *filename, int n);
void insert_line(const char *filename, const char *text, const char *save_file);
void edit_line(const char *filename, const char *text, int line, const char *save_file);

#endif
```

### sub_tool.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sub_tool.h"

// HELPER FUNCTION
static int count_lines(const char *filename)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
        return -1;

    char buffer[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL)
        count++;

    fclose(fptr);
    return count;
}

void view_all(const char *filename)
{
    FILE* fptr = fopen(filename,"r");
    if(fptr == NULL){
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }
    char buffer[MAX_LINE_LENGTH];
    while(fgets(buffer,MAX_LINE_LENGTH,fptr) != NULL)
    {
        printf("%s",buffer);
    }
    fclose(fptr);
}

void view_specific_line(const char *filename, int line)
{
    int total = count_lines(filename);

    if (total == -1)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    FILE *fptr = fopen(filename, "r");

    if (line <= 0 || line > total) 
    {
        fprintf(stderr, "Error: Line out of bounds.\n");
        fclose(fptr);
        exit(0);
    }

    char buffer[MAX_LINE_LENGTH];
    int current_line = 1;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (current_line == line)
        {
            printf("%s", buffer);
            fclose(fptr);
            return;
        }
        current_line++;
    }
    fclose(fptr);
}

void view_first_lines(const char *filename, int n)
{
    int total = count_lines(filename);

    if (total == -1)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    // if (n < 1)
    // {
    //     fprintf(stderr, "Error: Line out of bounds.\n");
    //     exit(0);
    // }

    if (n > total)
    {
        fprintf(stderr, "Error: File limit exceeded.\n");
        exit(0);
    }

    FILE *fptr = fopen(filename, "r");

    char buffer[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL && count < n)
    {
        printf("%s", buffer);
        count++;
    }

    fclose(fptr);
}

void view_last_lines(const char *filename, int n)
{
    int total = count_lines(filename);

    if (total == -1)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    // if (n < 1)
    // {
    //     fprintf(stderr, "Error: Line out of bounds.\n");
    //     exit(0);
    // }

    if (n > total)
    {
        fprintf(stderr, "Error: File limit exceeded.\n");
        exit(0);
    }

    FILE *fptr = fopen(filename, "r");

    char buffer[MAX_LINE_LENGTH];
    int current_line = 1;
    int start_line = total - n + 1;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL)
    {
        if (current_line >= start_line)
            printf("%s", buffer);

        current_line++;
    }

    fclose(fptr);
}

void insert_line(const char *filename,const char *text,const char *save_file)
{
    FILE *in = fopen(filename, "r");
    if (in == NULL)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    const char *out_name = (save_file != NULL) ? save_file : "temp.txt";

    FILE *out = fopen(out_name, "w");
    if (out == NULL)
    {
        fclose(in);
        exit(0);
    }

    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, MAX_LINE_LENGTH, in) != NULL)
        fputs(buffer, out);

    fprintf(out, "%s\n", text);

    fclose(in);
    fclose(out);

    if (save_file == NULL)
    {
        remove(filename);
        rename("temp.txt", filename);
    }
}

void edit_line(const char *filename,const char *text,int line,const char *save_file)
{
    int total = count_lines(filename);

    if (total == -1)
    {
        fprintf(stderr, "Error: File does not exist.\n");
        exit(0);
    }

    if (line < 1 || line > total)
    {
        fprintf(stderr, "Error: Line out of bounds.\n");
        exit(0);
    }

    FILE *in = fopen(filename, "r");
    const char *out_name = (save_file != NULL) ? save_file : "temp.txt";
    FILE *out = fopen(out_name, "w");

    char buffer[MAX_LINE_LENGTH];
    int current = 1;

    while (fgets(buffer, MAX_LINE_LENGTH, in) != NULL)
    {
        if (current == line)
            fprintf(out, "%s\n", text);
        else
            fputs(buffer, out);

        current++;
    }

    fclose(in);
    fclose(out);

    if (save_file == NULL)
    {
        remove(filename);
        rename("temp.txt", filename);
    }
}
```

### Makefile

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = sub_tool
OBJ = main.o sub_tool.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.c sub_tool.h
	$(CC) $(CFLAGS) -c main.c

sub_tool.o: sub_tool.c sub_tool.h
	$(CC) $(CFLAGS) -c sub_tool.c

clean:
	rm -f $(TARGET) $(OBJ)
```
