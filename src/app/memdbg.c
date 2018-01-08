#include <windows.h>

#include "bin_tree.h"

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
    void*  buf,
    size_t size)
{
    if (*(uint*)((char*)buf + size) != MEMDBG_TAG) {
        /*LOG THIS ERROR TO FILE IN THE FUTURE*/
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

    /*LOG THIS ERROR TO FILE IN THE FUTURE*/
    CheckOverflow(node->data.addr, node->data.size);
    free(node->data.addr);
    free(node);
}


void memdbg_deinit(void)
{
    BtFreeTree((BINARY_TREE*)g_dbgData.bufTree);

    return;
}

void* _dbg_malloc(
    _In_ size_t size,
    _In_ char*  srcFile,
    _In_ uint   fileLine)
{
    void*     buf;
    BUF_INFO* node;

    buf = malloc(size + sizeof(uint));
    if (!buf) {
        return NULL;
    }

    *(uint*)((char*)buf + size) = MEMDBG_TAG;

    node = (BUF_INFO*)malloc(sizeof(BUF_INFO));
    if (!node) {
        free(buf);
        return NULL;
    }

    node->addr = buf;
    node->size = size;
    node->dbgInfo.fileLine = fileLine;
    node->dbgInfo.srcFile = DuplString(srcFile);
    if (!node->dbgInfo.srcFile) {
        free(node);
        free(buf);
        return NULL;
    }

    g_dbgData.bufTree = BtAddNode((BINARY_TREE*)g_dbgData.bufTree,
        (BINARY_TREE*)g_dbgData.bufTree, node, sizeof(BUF_INFO));
    if (!g_dbgData.bufTree) {
        free(node->dbgInfo.srcFile);
        free(node);
        free(buf);
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
        /*LOG THIS ERROR TO FILE IN THE FUTURE*/
        return;
    }

    node = BtSearchNode((BINARY_TREE*)g_dbgData.bufTree, buf);
    if (!node) {
        /*LOG THIS ERROR TO FILE IN THE FUTURE*/
        return;
    }

    CheckOverflow(buf, node->data.size);

    free(buf);
    g_dbgData.bufTree = BtDeleteNode((BINARY_TREE*)g_dbgData.bufTree,
        node->data.addr);

    return;
}
