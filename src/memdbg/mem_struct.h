#ifndef mem_struct_h__
#define mem_struct_h__

typedef struct _MemBuf {
    void*  addr;
    size_t size;
    struct _dbginfo {
        char*  srcFile;
        size_t fileLine;
    } dbgInfo;
} BUF_INFO;

typedef struct {
    HANDLE hLogFile;
    void*  bufTree;
} MEMDBG;

#define MEMDBG_TAG (UINT32)'MDBG'

#endif // mem_struct_h__
