# Stack Memory Allocation

### Definition

Memory automatically allocated for **local (non-static) variables** inside functions.

### Characteristics

- Stored in the **stack segment**
- Allocation & deallocation are **automatic**
- Managed by the compiler
- Memory is freed when the function returns

### Example

```c
void func() {
int x=10;// stack memory
}
```

### Lifetime

From function entry → until function exits

### Advantages

- Fast allocation
- No manual memory management

### Limitations

- Limited size
- Cannot control lifetime
- Risk of stack overflow

# Static Memory Allocation

### Definition

Memory allocated for:

- **Global variables**
- **Static local variables**

### Characteristics

- Stored in the **data segment**
- Allocated **before program starts**
- Exists for entire program lifetime

### Example

```c
int global Var=5;// static memory

void func() {
static int count=0;// static memory
}
```

### Lifetime

From program start → until program ends

### Advantages

- Value persists between function calls
- No need to allocate manually

### Limitations

- Cannot resize
- Memory reserved even if not used

# Dynamic Memory Allocation

### Definition

Memory allocated **at runtime** using heap functions.

### Allocated in the Heap

Functions used:

```c
malloc()
calloc()
realloc()
free()
```

(Header required: `#include <stdlib.h>`)

### Example

```c
#include <stdlib.h>

int*ptr=malloc(sizeof(int));
*ptr=10;
free(ptr);
```

### Characteristics

- Stored in the **heap segment**
- Allocated and freed manually
- Size determined at runtime

### Lifetime

Until explicitly freed using `free()`

### Advantages

- Flexible size
- Useful for large data structures
- Memory control

### Limitations

- Must manually free memory
- Risk of memory leaks
- Slower than stack allocation

# Comparison Table

| Feature | Stack | Static | Heap |
| --- | --- | --- | --- |
| Managed by | Compiler | Compiler | Programmer |
| Lifetime | Function scope | Entire program | Until `free()` |
| Resize | ❌ No | ❌ No | ✔ Yes (`realloc`) |
| Speed | Very fast | Fast | Slower |
| Risk | Stack overflow | Wasted memory | Memory leaks |
| Example | `int x;` | `static int x;` | `malloc()` |

# Memory Layout (Simplified)

```
-----------------
|   Stack       |  ← Local variables
-----------------
|   Heap        |  ← Dynamic memory
-----------------
|   Data        |  ← Static & global
-----------------
|   Code        |  ← Program instructions
-----------------
```
