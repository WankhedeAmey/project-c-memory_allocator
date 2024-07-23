# project-c-memory_allocator
A memory allocator in C

This repository contains a custom memory allocator implemented in C. The memory allocator provides functionality for memory allocation and deallocation, similar to the standard malloc, free, calloc, and realloc functions. Additionally, it includes a debugging function print_mem_list to help visualize the memory blocks in use.

## Features
- Implements memory allocation functions: malloc, free, calloc, and realloc.
- Supports thread-safe memory allocation with the use of mutex locks.
- Provides a debugging function print_mem_list to display the memory blocks in use.

## Compilation and Usage
To compile the code and create a shared library:
```
gcc -o memalloc.so -fPIC -shared memalloc.c -lpthread
```
To use the custom memory allocator in your C programs, you need to include the header memalloc.h and link against the shared library memalloc.so.

To compile a program that uses the custom memory allocator:
```
gcc -o your_program your_program.c memalloc.so -lpthread
```
Replace your_program with the desired name for your compiled program and your_program.c with the source file containing your program code.

## Usage Example
A test file named "test.c" is included in repository.
Compile using
```
gcc -o test test.c memalloc.so -lpthread
```

The output of the program will show all the blocks of  memory beings created in heap.
