# Operating Systems & C/C++ Fundamentals Notes

## 1. Variadic Functions & Templates

### 🔹 Variadic Function (C / C++)

A function that accepts a **variable number of arguments**.

```c
#include <stdarg.h>

int sum(intcount, ...) {
va_list args;
va_start(args,count);

int total=0;
for (int i=0;i < count;i++) {
total+=va_arg(args,int);
    }

va_end(args);
return total;
}
```

### 🔹 Variadic Template (C++)

Type-safe version using templates:

```c
template<typename...Args>
auto sum(Args...args) {
return (args+ ...);
}
```

| Feature | Variadic Function | Variadic Template |
| --- | --- | --- |
| Language | C / C++ | C++ only |
| Type Safety | ❌ No | ✅ Yes |
| Checked At | Runtime | Compile-time |

## 2. Struct in C vs C++

| Feature | C | C++ |
| --- | --- | --- |
| Nature | Data container only | Same as class |
| Functions | Not allowed | Allowed |
| Constructors | ❌ | ✅ |
| Inheritance | ❌ | ✅ |
| Keyword required | `struct` needed | Not needed |
| Default access | Public | `struct → public`, `class → private` |

## 3. Memory Allocation in C

### 🔹 Types of Memory

| Type | Description |
| --- | --- |
| Stack | Local variables (automatic) |
| Heap | Dynamic allocation |
| Static | Global & static variables |

### 🔹 Dynamic Allocation Functions

```c
malloc()  // allocate (uninitialized)
calloc( ) // allocate + zero initialize
realloc() // resize memory
free() // deallocate
```

Example:

```c
int*arr= (int*)malloc(10*sizeof(int));
free(arr);
```

## 4. Void Pointer vs Typed Pointer

| Feature | `int*` | `void*` |
| --- | --- | --- |
| Type known | Yes | No |
| Dereferencing | Direct | Requires casting |
| Use case | Specific data | Generic programming |

### 🔹 Why `void*` exists?

- Generic memory handling
- Used in libraries (e.g., `malloc`)

## 5. Compilation Process

```
.c file → Preprocessor → Compiler → Assembler → Linker → Executable
```

### 🔹 Preprocessing Phase

- `#include` expansion
- `#define` macros replacement
- Comments removal
- Conditional compilation

## 6. Inline Functions vs Macros

| Feature | Macros | Inline |
| --- | --- | --- |
| Stage | Preprocessing | Compilation |
| Debugging | Hard | Easier |
| Type Safety | No | Yes |

## 7. Input / Output in C

### 🔹 Basic Functions

```c
printf("Value: %d",x);
scanf("%d",&x);
```

### 🔹 Format Specifiers

| Specifier | Meaning |
| --- | --- |
| `%d` | Integer |
| `%f` | Float |
| `%c` | Character |
| `%s` | String |
| `%p` | Pointer |

## 8. Data Types

| Type | Size | Description |
| --- | --- | --- |
| char | 1 byte | Character |
| int | 4 bytes | Integer |
| float | 4 bytes | Floating point |
| double | 8 bytes | High precision |

## 9. Control Structures

### 🔹 Logical Rules

- `0 → false`
- `non-zero → true`

### 🔹 Short Circuiting

```c
if (A || B)  // B not evaluated if A is true
if (A && B)  // B not evaluated if A is false
```

## 10. Loops

```c
for(int i=0; i<3 ;i++) { }

while(condition) { }
```

## 11. Arrays & Strings

### 🔹 Arrays

```c
int arr[10];
arr[0]=5;
```

### 🔹 Strings

- Array of characters ending with `\0`

```c
char name[]="Habiba";
```

## 12. Pointers

### 🔹 Basics

```c
int x=10;
int*p=&x;
```

### 🔹 Key Concepts

- Pointer stores **address**
- `p` → value
- `p` → address

### 🔹 Swap Example

Wrong (pass by value):

```c
void swap(int a,int b)
```

Correct (pass by reference):

```c
void swap(int*a,int*b)
```

## 13. Command-Line Arguments

```c
int main(int argc,char*argv[])
```

- `argc` → number of arguments
- `argv` → array of strings

Example:

```
./program 510
```

## 14. Structures (Data Structures)

```c
struct student {
char name[50];
int id;
};
```

Access:

```c
s.id=10;    // object
p->id=10;   // pointer
```

## 15. Memory Best Practices

- Always `free()` allocated memory
- Use `sizeof()` (never hardcode sizes)
- Avoid memory leaks

## 16. Compilation & Debugging

### 🔹 Compile

```
gcc file.c-o output
```

### 🔹 Run

```
./output
```

### 🔹 Debugging Tools

- `gdb`
- VSCode + CodeLLDB

## 17. Programming Best Practices

- Avoid magic numbers → use `#define`
- Use meaningful variable names
- Keep functions small
- Use indentation
- Comment important logic
- Plan before coding

## 18. C vs C++

| Feature | C | C++ |
| --- | --- | --- |
| Paradigm | Procedural | OOP + Procedural |
| Classes | ❌ | ✅ |
| Templates | ❌ | ✅ |
| Type Safety | Less | More |
