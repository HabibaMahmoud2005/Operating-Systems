# 📘 Makefiles, Build Process, and Debugging

## 1. Why Do We Need Makefiles?

When a project contains multiple source files, compiling manually becomes inefficient.

Example:

```bash
gcc main.c hello.c factorial.c -o hello
```

If only one file changes, recompiling everything wastes time.

A **Makefile** allows automatic project building by compiling only the files that changed.

Advantages:

- Automatic compilation
- Faster build time
- Dependency tracking
- Easy project management
- Standard tool in Linux / Unix

The tool used to execute Makefiles is:

```
make
```

## 2. The Build Process (Compilation Model)

In C/C++ projects, building a program happens in two main stages.

### Step 1: Compilation

The compiler converts each source file into an object file.

```
gcc -c main.c
```

Output:

```
main.o
```

Each `.c` file → `.o` file

### Step 2: Linking

The linker combines object files into an executable.

```
gcc main.o hello.o factorial.o -o hello
```

Output:

```
hello   (executable)
```

Summary:

```
Source (.c) → Object (.o) → Executable
```

This separation allows recompiling only modified files.

## 3. The make Utility

The command:

```
make
```

does the following:

1. Looks for a file named `makefile` or `Makefile`
2. Reads the rules inside it
3. Executes the required commands

If the makefile has a different name:

```
make -f MyMakefile
```

Help:

```
man make
```

## 4. Makefile Basic Syntax

A Makefile consists of rules.

General form:

```
target: dependencies
    command
```

Important rules:

- Command must start with TAB
- Target = what we want to build
- Dependencies = files needed
- Command = how to build

Example:

```
all:
    gcc main.c hello.c factorial.c -o hello
```

Here:

- target = all
- no dependencies
- command = compile everything

If no target is specified, make runs the first target.

## 5. Targets and Dependencies

Better Makefile:

```
all: hello

hello: main.o hello.o factorial.o
    gcc main.o hello.o factorial.o -o hello
```

Object file rules:

```
main.o: main.c
    gcc -c main.c

hello.o: hello.c
    gcc -c hello.c

factorial.o: factorial.c
    gcc -c factorial.c
```

Now make will only compile what changed.

Example:

If `hello.c` changes → only hello.o rebuilt.

## 6. Why Dependencies Are Important

Without dependencies:

```
gcc main.c hello.c factorial.c -o hello
```

Everything recompiles every time.

With dependencies:

make checks timestamps:

- If file changed → rebuild
- If not changed → skip

This makes large projects fast.

## 7. Clean Target

Often we need to remove generated files.

```
clean:
    rm -f *.o hello
```

Run:

```
make clean
```

Purpose:

- remove object files
- remove executable
- rebuild from scratch

This target is not automatic, you must call it.

## 8. Using Variables in Makefiles

Variables make Makefiles flexible.

Example:

```
CC = gcc
CFLAGS = -c -Wall
```

Use variables:

```
$(CC)
$(CFLAGS)
```

Example:

```
CC=gcc
CFLAGS=-c -Wall

hello: main.o hello.o factorial.o
    $(CC) main.o hello.o factorial.o -o hello
```

Advantages:

- easier to modify compiler
- reusable code
- cleaner Makefile

## 9. Complete Example for Makefile

```
CC = gcc
CFLAGS = -c -Wall
LDFLAGS =

SOURCES = main.c hello.c factorial.c
OBJECTS = $(SOURCES:.c=.o)

EXECUTABLE = hello

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    $(CC) $(OBJECTS) -o $@

.c.o:
    $(CC) $(CFLAGS) $< -o $@

clean:
    rm -f $(OBJECTS) $(EXECUTABLE)
```

Explanation:

- SOURCES = list of .c files
- OBJECTS = convert to .o
- pattern rule builds .o automatically

## 10. Automatic Variables in Makefiles

| Symbol | Meaning |
| --- | --- |
| $@ | target name |
| $< | first dependency |
| $^ | all dependencies |
| $? | changed dependencies |

Example:

```
$(CC) $^ -o $@
```

means

```
gcc all_objects -o target
```

## 11. Pattern Rules

Pattern rule:

```
.c.o:
    gcc -c $< -o $@
```

Meaning:

If `.o` needed, build from `.c`.

Example:

```
main.c → main.o
hello.c → hello.o
```

This avoids writing rules for every file.

## 12. How make Decides to Rebuild

make checks:

1. Does target exist?
2. Are dependencies newer?
3. Are dependencies missing?

Rules:

- missing file → build
- newer source → rebuild
- everything ok → do nothing

Example:

```
hello depends on main.o
main.o depends on main.c
```

If main.c updated → main.o rebuilt → hello relinked.

## 13. GCC Important Options

Compile only:

```
gcc -c file.c
```

Warnings:

```
gcc -Wall file.c
```

Debug info:

```
gcc -g file.c
```

Output name:

```
gcc file.c -o prog
```

Multiple files:

```
gcc a.c b.c -o prog
```

## 14. Debugging with gdb

Compile with debug info:

```
gcc -g main.c -o prog
```

Start debugger:

```
gdb ./prog
```

Run program:

```
run
r
```

Run with arguments:

```
run arg1 arg2
```

## 15. Breakpoints

Set breakpoint at line:

```
b 10
```

Set breakpoint at function:

```
b main
```

Show breakpoints:

```
info break
```

Delete:

```
delete 1
delete
```

## 16. Execution Control in gdb

Continue execution:

```
c
```

Step into:

```
s
```

Next line:

```
n
```

Finish function:

```
finish
```

## 17. Inspecting Variables

Print variable:

```
p x
```

Change variable:

```
set var x=5
```

Watch variable:

```
watch x
```

Show watches:

```
info watch
```

## 18. Backtrace and Stack

Show call stack:

```
backtrace
where
```

Show frame:

```
frame
```

Used when program crashes.

## 19. Show Source Code in gdb

Show code:

```
list
```

Show function:

```
l main
```

## 20. Exit gdb

```
quit
q
```
