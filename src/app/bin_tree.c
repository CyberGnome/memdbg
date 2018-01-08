#include <windows.h>

#include "bin_tree.h"

BINARY_TREE* BtAddNode(
    _Inout_  BINARY_TREE *root,
    _In_opt_ BINARY_TREE *node,
    _In_     void*  data,
    _In_     size_t dataSize)
{
    if (!node) {
        node = (BINARY_TREE*)malloc(sizeof(BINARY_TREE));
        if (!node) {
            return NULL;
        }

        node->left = NULL;
        node->right = NULL;
        memcpy(&node->data, data, dataSize);

        if (!root) {
            return node;
        }
        if (((BUF_INFO*)data)->addr < root->data.addr) {
            root->left = node;
        } else {
            root->right = node;
        }

        return node;
    }

    if (((BUF_INFO*)data)->addr < node->data.addr) {
        BtAddNode(node, node->left, data, dataSize);
    } else {
        BtAddNode(node, node->right, data, dataSize);
    }

    return root; 
}

BINARY_TREE* BtSearchNode(
    _In_ BINARY_TREE *root,
    _In_ void* memAddr)
{
    if(!root) {
        return root;
    }

    while (root->data.addr != memAddr) {
        if (memAddr < root->data.addr) {
            root = root->left;
        } else {
            root = root->right;
        } if (!root) {
            break;
        }
    }
    return root;
}

BINARY_TREE* BtDeleteNode(
    _In_ BINARY_TREE* root,
    _In_ void*        memAddr)
{
    BINARY_TREE *p;
    BINARY_TREE *p2;

    if (!root) {
        return root;
    }

    if (root->data.addr == memAddr) {
        if (root->left == root->right) {
            free(root);
            return NULL;

        } else if (root->left == NULL) {
            p = root->right;
            free(root);
            return p;

        } else if(root->right == NULL) {
            p = root->left;
            free(root);
            return p;

        } else {
            p2 = root->right;
            p = root->right;

            while (p->left) {
                p = p->left;
            }

            p->left = root->left;
            free(root);

            return p2;
        }
    }

    if(root->data.addr < memAddr) {
        root->right = BtDeleteNode(root->right, memAddr);
    } else {
        root->left = BtDeleteNode(root->left, memAddr);
    }

    return root;
}
