#include <windows.h>

#include "bin_tree.h"
#include "logs.h"

MEMDBG g_dbgData;

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

void memdbg_init(void)
{
    memset(&g_dbgData, 0, sizeof(MEMDBG));
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


void memdbg_deinit(void)
{
    BtFreeTree((BINARY_TREE*)g_dbgData.bufTree);
    if (g_dbgData.hLogFile) {
        MdbgCloseLogFile(g_dbgData.hLogFile);
    }
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
    size_t    fullSize = size + num;

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

    if (!g_dbgData.bufTree) {
        MdbgLoggingBug(&g_dbgData.hLogFile,
            NULL, 0, srcFile, fileLine, UNALLOC_MEMORY);
        return;
    }

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
