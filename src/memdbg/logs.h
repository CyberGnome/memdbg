#ifndef logs_h__
#define logs_h__

int MdbgCreateLogFile(
    _In_  char*   fileName,
    _Out_ HANDLE* hFile);

void MdbgCloseLogFile(
    _In_ HANDLE hFile);

int MdbgLoggingBug(
    _Inout_  HANDLE* hFile,
    _In_     char*   fileAllocatedMem,
    _In_     size_t  lineAllocatedMem,
    _In_opt_ char*   fileFailedMem,
    _In_opt_ size_t  lineFailedMem,
    _In_     size_t  bugFlag);

// log file
#define LOG_FILE (char*)"MEMORY_BUGS.log"

// error codes
#define MEMORY_LEAK    (size_t)0xBADC0DED
#define OVERFLOW       (size_t)0xBADC0C0A
#define UNALLOC_MEMORY (size_t)0xDEADC0DE

// strings
#define mem_leaks_bug    (char*)"Memory Leak"
#define buf_overflow_bug (char*)"Buffer overflow at file "
#define access_mem_bug   (char*)"Trying to access unallocated memory "

#define memory_alloc     (char*)"Memory was allocated at file "

#define unknown_file     (char*)"<unknown>"

#define car_ret_1        (char*)"\r\n"
#define car_ret_2        (char*)"-----------------\r\n\r\n"
#define delimiter        (char*)":"

#endif // logs_h__
