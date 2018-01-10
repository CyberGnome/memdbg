#ifndef memdbg_h__
#define memdbg_h__

void memdbg_init(void);
void memdbg_deinit(void);

void* _dbg_malloc(
    _In_ size_t       size,
    _In_ char*        srcFile,
    _In_ unsigned int fileLine);

void* _dbg_calloc(
    _In_ size_t       num,
    _In_ size_t       size,
    _In_ char*        srcFile,
    _In_ unsigned int fileLine);

void _dbg_free(
    _In_ void*        buf,
    _In_ char*        srcFile,
    _In_ unsigned int fileLine);

void* _dbg_memcpy(
    _In_ void*        destptr,
    _In_ const void*  srcptr,
    _In_ size_t       num,
    _In_ char*        srcFile,
    _In_ unsigned int fileLine);

errno_t _dbg_memcpy_s(
    _In_ void*        destptr,
    _In_ size_t       destSize,
    _In_ const void*  srcptr,
    _In_ size_t       count,
    _In_ char*        srcFile,
    _In_ unsigned int fileLine);


#define dbg_calloc(_num, _size)                               /* calloc   */ \
    _dbg_calloc((_num), (_size), __FILE__, __LINE__)

#define dbg_malloc(_size)                                     /* malloc   */ \
    _dbg_malloc((_size), __FILE__, __LINE__)

#define dbg_free(_buf)                                        /* free     */ \
    _dbg_free((_buf), __FILE__, __LINE__)

#define dbg_memcpy(_destptr, _srcptr, _num)                   /* memcpy   */ \
    _dbg_memcpy((_destptr), (_srcptr), (_num), __FILE__, __LINE__)

#define dbg_memcpy_s(_destptr, _destSize, _srcptr, _count)    /* memcpy_s */ \
    _dbg_memcpy_s((_destptr), (_destSize), (_srcptr), (_count), __FILE__, __LINE__)




#define DBGSTATUS_BUFFER_OVERFLOW           ((errno_t)0x80000005L)
#define DBGSTATUS_MEMORY_NOT_ALLOCATED      ((errno_t)0xC00000A0L)
#define DBGSTATUS_SUCCESS                   ((errno_t)0x00000000L)
#endif // memdbg_h__
