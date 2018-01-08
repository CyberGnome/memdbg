#ifndef mem_struct_h__
#define mem_struct_h__

typedef unsigned int uint;

typedef struct _MemBuf {
    void*  addr;
    size_t size;
    struct _dbginfo {
        char* srcFile;
        int   fileLine;
    } dbgInfo;
} BUF_INFO;

typedef struct {
    HANDLE hLogFile;
    void*  bufTree;
} MEMDBG;

#define MEMDBG_TAG (uint)'MDBG'

#define LOG_FILE (char*)"MEMORY_BUGS.log"

#endif // mem_struct_h__
