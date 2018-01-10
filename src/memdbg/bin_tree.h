#ifndef bin_tree_h__
#define bin_tree_h__

#include "mem_struct.h"

typedef unsigned char byte;

typedef struct _tree {
    struct _tree* left;
    struct _tree* right;
    BUF_INFO      data;
} BINARY_TREE;

BINARY_TREE* BtAddNode(
    _Inout_  BINARY_TREE *root,
    _In_opt_ BINARY_TREE *node,
    _In_     void*  data,
    _In_     size_t dataSize);

BINARY_TREE* BtSearchNode(
    _In_ BINARY_TREE *root,
    _In_ void* memAddr);

BINARY_TREE* BtSearchNodeInRange(
    _In_ BINARY_TREE* root,
    _In_ void*        memAddr);

BINARY_TREE* BtDeleteNode(
    _In_ BINARY_TREE* root,
    _In_ void*        memAddr);

#endif // bin_tree_h__
