# memdbg
Library for detecting bugs with memory allocation on a heap.

## Functional
1. Detecting memory leaks when you allocated memory and forgot to free it
1. Detecting the access to unallocated memory
1. Detecting of buffer overflows when copying and moving data

## Description

**memdbg.lib** designed to help developers fix memory bugs during the testing phase.

Library contain wrap functions of basic functions of interaction with memory. It is wraps must use only in debug versions of their products. It is perform a dynamic search for memory bugs.

If buffer overflow occurs, memory leak or attempt to access unallocated memory, it record the error information in the log file.

Sample error message:
```
Trying to access unallocated memory C:\Projects\ProjectName\src\inits.c:54
-----------------

Buffer overflow at file C:\Projects\ProjectName\src\fltr_ops.c:101
Memory was allocated at file C:\Projects\ProjectName\src\inits.c:1029
-----------------

Memory Leak
Memory was allocated at file C:\Projects\ProjectName\src\content.c:697
-----------------

```

You can see error description and place where the mistake was made.

The format of the error message:

```
Error_Description <file>:<line_in_file>
```

Also you can see the place where memory was allocated in the next line (of course, if memory was allocated).

### Use of the library

You must use this library only for *debug* build! This library help to found and fix memory bugs.

You should not use this library in *release* build! You can use macros for definitions these functions only for *debug* builds.

Sample of definitions:

```
  #ifdef _DEBUG
    #define MALLOC(_size) dbg_malloc((_size))
    #define FREE(_memory) dbg_free((_memory))
  #else
    #define MALLOC(_size) malloc((_size))
    #define FREE(_memory) free((_memory))
  #endif

```

Now if you build *Debug* version the compiler will compile *dbg_malloc* and *dbg_free* functions and if you build *Release* version the compiler will compile *malloc* and *free* functions.

**Feature of using memdbg-functions:**
* You must initialize **memdbg** by calling *memdbg_init()* function before using *dbg_malloc* and other functions from this library.
* You must deinitialize **memdbg** by calling *memdbg_deinit()* function before exit application. It's help to find all memory leaks in the program.
* It is **forbidden** to reallocate or free memory using standard functions for memory that was allocated using *memdbg-functions*.
* It is **not recommended** to use others standard memory functions for memory that was allocated using *memdbg-functions*. (*But it is **not forbidden**!*)

*An example of using the library is shown in the project **test_app**.*

## Future plans

* Adding new wraps of standard memory functions
* Unix support
* Support kernel memory functions.

# Releases
### memdbg.lib (version 0.1)

Object             | Description
-------------------|------------------------
**Release Data**   | 12.01.2018
**Supported OS**   | *Windows 7 - Windows 10 (x86/x64)* [desktop apps]
**Functions list** | *dbg_malloc(_size)*, *dbg_calloc(_count, _size)*, *dbg_free(_memory)*, *dbg_realloc(_memory, _size)*, *dbg_memcpy(_dst, _src, _size)*, *dbg_memcpy_s(_dst, _dstSize, _src, _maxCount)*, *dbg_memmove(_dst, _src, _size)*, *dbg_memmove_s(_dst, _dstSize, _src, _maxCount)*, *dbg_memset(_dst, _val, _size)*
