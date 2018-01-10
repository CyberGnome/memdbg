#ifndef memdbg_h__
#define memdbg_h__

void memdbg_init(void);
void memdbg_deinit(void);

void* _dbg_malloc(
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ size_t fileLine);

void* _dbg_calloc(
    _In_ size_t count,
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ size_t fileLine);

void _dbg_free(
    _In_ void*  memory,
    _In_ char*  srcFile,
    _In_ size_t fileLine);

void* _dbg_realloc(
    _In_ void*  memory,
    _In_ size_t newSize,
    _In_ char*  srcFile,
    _In_ size_t fileLine);

void* _dbg_memcpy(
    _In_ void*       dst,
    _In_ const void* src,
    _In_ size_t      size,
    _In_ char*       srcFile,
    _In_ size_t      fileLine);

errno_t _dbg_memcpy_s(
    _In_ void*       dst,
    _In_ rsize_t     dstSize,
    _In_ const void* src,
    _In_ rsize_t     maxCount,
    _In_ char*       srcFile,
    _In_ size_t      fileLine);

void* _dbg_memset(
    _In_ void*  dst,
    _In_ int    val,
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ size_t fileLine);

void* _dbg_memmove(
    _In_ void*       dst,
    _In_ const void* src,
    _In_ size_t      size,
    _In_ char*       srcFile,
    _In_ size_t      fileLine);

errno_t _dbg_memmove_s(
    _In_ void*       dst,
    _In_ rsize_t     dstSize,
    _In_ const void* src,
    _In_ rsize_t     maxCount,
    _In_ char*       srcFile,
    _In_ size_t      fileLine);

#define dbg_calloc(_count, _size)                                /* calloc   */ \
    _dbg_calloc((_count), (_size), __FILE__, __LINE__)

#define dbg_malloc(_size)                                        /* malloc   */ \
    _dbg_malloc((_size), __FILE__, __LINE__)

#define dbg_free(_memory)                                        /* free     */ \
    _dbg_free((_memory), __FILE__, __LINE__)

#define dbg_realloc(_memory, _size)                              /* realloc  */ \
    _dbg_realloc((_memory), (_size), __FILE__, __LINE__)

#define dbg_memcpy(_dst, _src, _size)                            /* memcpy   */ \
    _dbg_memcpy((_dst), (_src), (_size), __FILE__, __LINE__)

#define dbg_memcpy_s(_dst, _dstSize, _src, _maxCount)            /* memcpy_s */ \
    _dbg_memcpy_s((_dst), (_dstSize), (_src), (_maxCount), __FILE__, __LINE__)

#define dbg_memmove(_dst, _src, _size)                           /* memmove   */ \
    _dbg_memmove((_dst), (_src), (_size), __FILE__, __LINE__)

#define dbg_memmove_s(_dst, _dstSize, _src, _maxCount)           /* memmove_s */ \
    _dbg_memmove_s((_dst), (_dstSize), (_src), (_maxCount), __FILE__, __LINE__)

#define dbg_memset(_dst, _val, _size)                            /* memset   */ \
    _dbg_memset((_dst), (_val), (_size), __FILE__, __LINE__)



/* specific statuses for errorno_t */
#define DBGSTATUS_BUFFER_OVERFLOW           ((errno_t)0x80000005L)
#define DBGSTATUS_MEMORY_NOT_ALLOCATED      ((errno_t)0xC00000A0L)
#define DBGSTATUS_SUCCESS                   ((errno_t)0x00000000L)

#endif // memdbg_h__
