#include "index_tree.h"

#include <stdlib.h>
pIndexTree CreateIndexTree(int item_index, PriorityCmp maintain_cmp,
                           PriorityCmp query_cmp) {
    pIndexTree new_index_tree;
    new_index_tree = (pIndexTree)malloc(sizeof(sIndexTree));
    new_index_tree->query_cmp = query_cmp;
    new_index_tree->maintain_cmp = maintain_cmp;
    new_index_tree->root = NULL;
    new_index_tree->item_index = item_index;
    return new_index_tree;
}

pTreeNode IndexTreeBegin(pIndexTree index_tree) {
    pTreeNode current;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

void DestructIndexTree(void* index_tree) {
    if (((pIndexTree)index_tree)->root != NULL)
        DestructTree(((pIndexTree)index_tree)->root);
    free(index_tree);
}

pTreeNode FindIndexOfKeyInIndexTree(pIndexTree index_tree, const char* key) {
    return FindFirstTreeOfKey(index_tree->root, key, index_tree->query_cmp);
}

pTreeNode FindNextIndexOfKeyInIndexTree(pIndexTree index_tree, const char* key,
                                        pTreeNode node) {
    return FindNextTreeOfKey(key, node, index_tree->query_cmp);
}

void RedirectIndexTo(pIndexTree index_tree, pDataBlockContainer _old,
                     pDataBlockContainer _new) {
    pTreeNode tmp;
    tmp = FindTreeOfKey(index_tree->root, _old, index_tree->maintain_cmp);
    if (tmp != NULL) tmp->key = _new;
}
void LoadIndexToIndexTree(pIndexTree index_tree, pDataBlockContainer key) {
    InsertNode(&(index_tree->root), key, index_tree->maintain_cmp);
}
void UnloadIndexFromIndexTree(pIndexTree index_tree, pDataBlockContainer key) {
    RemoveNodeFromRoot(&(index_tree->root), key, index_tree->maintain_cmp);
}
