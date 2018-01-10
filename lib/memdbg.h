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

#define dbg_calloc(_num, _size)                        /* calloc */ \
    _dbg_calloc((_num), (_size), __FILE__, __LINE__)

#define dbg_malloc(_size)                              /* malloc */ \
    _dbg_malloc((_size), __FILE__, __LINE__)

#define dbg_free(_buf)                                 /* free   */ \
    _dbg_free((_buf), __FILE__, __LINE__)

#define dbg_memcpy(_destptr, _srcptr, _num)            /* memcpy */ \
    _dbg_memcpy((_destptr), (_srcptr), (_num), __FILE__, __LINE__)

#endif // memdbg_h__
