#ifndef __SBT__
#define __SBT__


typedef struct SizeBalancedTree {
    void* key;
    struct SizeBalancedTree* left;
    struct SizeBalancedTree* right;
    struct SizeBalancedTree* parent;
    struct SizeBalancedTree* succ;
    struct SizeBalancedTree* pred;
    int size;
} sTreeNode;

typedef sTreeNode* pTreeNode;

/**
 * @brief compare the priority of two key values
 * @return 1 if a is prior to b; 0 if a is the same prior to b; -1 if b is prior
 * to a
 */
typedef int (*PriorityCmp)(const void* a, const void* b);

void DestructTree(pTreeNode target);

void InsertNodeFromRoot(pTreeNode* target, void* key, PriorityCmp cmp);

pTreeNode RemoveDirectly(pTreeNode root);

void RemoveNodeByKey(pTreeNode* root, const void* key, PriorityCmp cmp);

pTreeNode FindTreeOfKey(pTreeNode current, const void* key, PriorityCmp cmp);
pTreeNode FindFirstTreeOfKey(pTreeNode current, const void* key, PriorityCmp cmp);
pTreeNode FindNextTreeOfKey(const void* key, pTreeNode last, PriorityCmp cmp);

pTreeNode GetPredeccessorIn(pTreeNode current, const void* key,
                            PriorityCmp cmp);

pTreeNode GetSuccessorIn(pTreeNode current, const void* key, PriorityCmp cmp);

#endif