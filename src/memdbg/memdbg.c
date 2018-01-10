#include <windows.h>

#include "bin_tree.h"
#include "memdbg.h"
#include "logs.h"

#define UNKNOWN_BUFFER_SIZE (size_t)-1

MEMDBG g_dbgData;



/************************************************************************/
/*                        service functions                             */
/************************************************************************/

static
char* DuplString(
    _In_ char* srcStr)
{
    size_t size = strlen(srcStr) + 1;
    char*  dstStr = (char*)malloc(size);

    if (!dstStr) {
        return NULL;
    }
    memset(dstStr, 0, size);
    memcpy(dstStr, srcStr, size - 1);

    return dstStr;
}

static
void CheckOverflow(
    void*        buf,
    size_t       size,
    char*        srcFile,
    unsigned int fileLine)
{
    if (*(uint*)((char*)buf + size) != MEMDBG_TAG) {
        MdbgLoggingBug(&g_dbgData.hLogFile,
            srcFile, fileLine, NULL, 0, OVERFLOW);
    }

    return;
}

static
void BtFreeTree (
    _In_ BINARY_TREE* node) {
        if (!node) {
            return;
        }

        BtFreeTree(node->left);
        BtFreeTree(node->right);

        MdbgLoggingBug(&g_dbgData.hLogFile,
            node->data.dbgInfo.srcFile,
            node->data.dbgInfo.fileLine,
            NULL, 0, MEMORY_LEAK);
        CheckOverflow(node->data.addr, node->data.size,
            node->data.dbgInfo.srcFile, node->data.dbgInfo.fileLine);
        free(node->data.addr);
        free(node->data.dbgInfo.srcFile);
        free(node);

        return;
}

static
int AddPoolToTree(
    _In_ void*  buf,
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ uint   fileLine)
{
    BUF_INFO* node;
    int       err = 0;

    node = (BUF_INFO*)malloc(sizeof(BUF_INFO));
    if (!node) {
        return GetLastError();
    }

    node->addr = buf;
    node->size = size;
    node->dbgInfo.fileLine = fileLine;
    node->dbgInfo.srcFile = DuplString(srcFile);
    if (!node->dbgInfo.srcFile) {
        err = GetLastError();
        free(node);
        goto _ret;
    }

    g_dbgData.bufTree = BtAddNode((BINARY_TREE*)g_dbgData.bufTree,
        (BINARY_TREE*)g_dbgData.bufTree, node, sizeof(BUF_INFO));
    if (!g_dbgData.bufTree) {
        free(node->dbgInfo.srcFile);
        free(node);
        err = -1;
    }

_ret:
    return err;
}

static
BOOLEAN CheckBuffer(
    _In_     void*   bufptr,
    _In_opt_ size_t  bufSize,
    _Inout_  size_t* copySize,
    _In_     char*   srcFile,
    _In_     uint    fileLine)
{
    BINARY_TREE* node;
    size_t       availableSize;

    node = BtSearchNodeInRange((BINARY_TREE*)g_dbgData.bufTree, bufptr);
    if (!node) {
        return FALSE;
    }

    availableSize = ((size_t)node->data.addr +
        node->data.size) - (size_t)bufptr;

    if (bufSize == UNKNOWN_BUFFER_SIZE) {
        if (availableSize < *copySize) {
            *copySize = availableSize;
            MdbgLoggingBug(&g_dbgData.hLogFile,
                node->data.dbgInfo.srcFile,
                node->data.dbgInfo.fileLine,
                srcFile, fileLine, OVERFLOW);
        }
    } else {
        if (availableSize < bufSize ||
            bufSize < *copySize) 
        {
            MdbgLoggingBug(&g_dbgData.hLogFile,
                node->data.dbgInfo.srcFile,
                node->data.dbgInfo.fileLine,
                srcFile, fileLine, OVERFLOW);
            return FALSE;
        }
    }

    return TRUE;
}



/************************************************************************/
/*               initializing/deinitializing functions                  */
/************************************************************************/

void memdbg_init(void)
{
    memset(&g_dbgData, 0, sizeof(MEMDBG));
    return;
}

void memdbg_deinit(void)
{
    BtFreeTree((BINARY_TREE*)g_dbgData.bufTree);
    if (g_dbgData.hLogFile) {
        MdbgCloseLogFile(g_dbgData.hLogFile);
    }
    return;
}



/************************************************************************/
/*                 allotting/cleansing memory functions                 */
/************************************************************************/

void* _dbg_malloc(
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ uint   fileLine)
{
    void*     buf;

    buf = malloc(size + sizeof(uint));
    if (!buf) {
        return NULL;
    }

    *(uint*)((char*)buf + size) = MEMDBG_TAG;

    if (AddPoolToTree(buf, size, srcFile, fileLine)) {
        free(buf);
        buf = NULL;
    }

    return buf;
}

void* _dbg_calloc(
    _In_ size_t num,
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ uint   fileLine)
{
    void*     buf;
    size_t    fullSize = size * num;

    buf = malloc(fullSize + sizeof(uint));
    if (!buf) {
        return NULL;
    }
    memset(buf, 0, fullSize);

    *(uint*)((char*)buf + fullSize) = MEMDBG_TAG;

    if (AddPoolToTree(buf, fullSize, srcFile, fileLine)) {
        free(buf);
        buf = NULL;
    }

    return buf;
}

void _dbg_free(
    _In_ void* buf,
    _In_ char* srcFile,
    _In_ uint  fileLine)
{
    BINARY_TREE* node = NULL;

    node = BtSearchNode((BINARY_TREE*)g_dbgData.bufTree, buf);
    if (!node) {
        MdbgLoggingBug(&g_dbgData.hLogFile,
            NULL, 0, srcFile, fileLine, UNALLOC_MEMORY);
        return;
    }

    CheckOverflow(buf, node->data.size, 
        node->data.dbgInfo.srcFile, node->data.dbgInfo.fileLine);

    free(buf);
    free(node->data.dbgInfo.srcFile);
    g_dbgData.bufTree = BtDeleteNode((BINARY_TREE*)g_dbgData.bufTree,
        node->data.addr);

    return;
}

void* _dbg_realloc(
    _In_ void*  ptr,
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ uint   fileLine)
{
    void*        dst;
    BINARY_TREE* node;

    node = BtSearchNodeInRange((BINARY_TREE*)g_dbgData.bufTree, ptr);
    if (!node) {
        return ptr;
    }

    dst = _dbg_malloc(size, srcFile, fileLine);
    if (!dst) {
        return ptr;
    }

    if (size <= node->data.size) {
        _dbg_memcpy(dst, ptr, size, srcFile, fileLine);
    } else {
        _dbg_memcpy(dst, ptr, node->data.size, srcFile, fileLine);
    }

    _dbg_free(ptr, srcFile, fileLine);

    return dst;
}



/************************************************************************/
/*                    work with memory functions                        */
/************************************************************************/

void* _dbg_memcpy(
    _In_ void*       destptr,
    _In_ const void* srcptr,
    _In_ size_t      num,
    _In_ char*       srcFile,
    _In_ uint        fileLine)
{
    if (!CheckBuffer(destptr, UNKNOWN_BUFFER_SIZE,
        &num, srcFile, fileLine))
    {
        return NULL;
    }

    return memcpy(destptr, srcptr, num);
}

errno_t _dbg_memcpy_s(
    _In_ void*       destptr,
    _In_ size_t      destSize,
    _In_ const void* srcptr,
    _In_ size_t      count,
    _In_ char*       srcFile,
    _In_ uint        fileLine)
{
    if (!CheckBuffer(destptr, destSize, &count, srcFile, fileLine)) {
        return DBGSTATUS_BUFFER_OVERFLOW;
    }

    return memcpy_s(destptr, destSize, srcptr, count);
}

void* _dbg_memset(
    _In_ void*  dst,
    _In_ int    value,
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ uint   fileLine)
{
    if (!CheckBuffer(dst, UNKNOWN_BUFFER_SIZE,
        &size, srcFile, fileLine))
    {
        return NULL;
    }

    return memset(dst, value, size);
}

void* _dbg_memmove(
    _In_ void*       destptr,
    _In_ const void* srcptr,
    _In_ size_t      num,
    _In_ char*       srcFile,
    _In_ uint        fileLine)
{
    if (!CheckBuffer(destptr, UNKNOWN_BUFFER_SIZE,
        &num, srcFile, fileLine))
    {
        return NULL;
    }

    return memmove(destptr, srcptr, num);
}

errno_t _dbg_memmove_s(
    _In_ void*       destptr,
    _In_ rsize_t     destSize,
    _In_ const void* srcptr,
    _In_ rsize_t     count,
    _In_ char*       srcFile,
    _In_ uint        fileLine)
{
    if (!CheckBuffer(destptr, destSize, &count, srcFile, fileLine)) {
        return DBGSTATUS_BUFFER_OVERFLOW;
    }

    return memmove_s(destptr, destSize, srcptr, count);
}
