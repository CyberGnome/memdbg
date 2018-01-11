#ifndef debug_h__
#define debug_h__

#include "memdbg.h"

#ifdef _DEBUG

#define MEMDBG_INIT()   memdbg_init()
#define MEMDBG_DEINIT() memdbg_deinit()

#define MALLOC(_size) \
    dbg_malloc((_size))

#define CALLOC(_count, _size) \
    dbg_calloc((_count), (_size))

#define FREE(_memory) \
    dbg_free((_memory))

#define REALLOC(_memory, _size) \
    dbg_realloc((_memory), (_size))

#define MEMCPY(_dst, _src, _size) \
    dbg_memcpy((_dst), (_src), (_size))

#define MEMCPY_S(_dst, _dstSize, _src, _size) \
    dbg_memcpy_s((_dst), (_dstSize), (_src), (_size))

#define MEMMOVE(_dst, _src, _size) \
    dbg_memmove((_dst), (_src), (_size))

#define MEMMOVE_S(_dst, _dstSize, _src, _size) \
    dbg_memmove_s((_dst), (_dstSize), (_src), (_size))

#define MEMSET(_dst, _val, _size) \
    dbg_memset((_dst), (_val), (_size))

#else

#define MEMDBG_INIT()
#define MEMDBG_DEINIT()

#define MALLOC(_size) malloc((_size))
#define CALLOC(_count, _size) calloc((_count), (_size))
#define FREE(_memory) free((_memory))
#define REALLOC(_memory, _size) realloc((_memory), (_size))
#define MEMCPY(_dst, _src, _size) memcpy((_dst), (_src), (_size))

#define MEMCPY_S(_dst, _dstSize, _src, _size) \
    memcpy_s((_dst), (_dstSize), (_src), (_size))

#define MEMMOVE(_dst, _src, _size) memmove((_dst), (_src), (_size))

#define MEMMOVE_S(_dst, _dstSize, _src, _size) \
    memmove_s((_dst), (_dstSize), (_src), (_size))

#define MEMSET(_dst, _val, _size) memset((_dst), (_val), (_size))

#endif



#endif // debug_h__
