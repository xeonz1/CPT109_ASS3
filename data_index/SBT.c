
#include "SBT.h"

#include "memory.h"
#include "stdlib.h"

static int getsize(pTreeNode target) {
    if (target == NULL)
        return 0;
    else
        return target->size;
}

void DestructTree(pTreeNode target) {
    if (target->left != NULL) DestructTree(target->left);
    if (target->right != NULL) DestructTree(target->right);
    free(target);
}
void LeftRotate(pTreeNode* target) {
    pTreeNode previous_son = (*target)->right;
    if (previous_son == NULL) return;
    (*target)->right = previous_son->left;
    previous_son->left = (*target);
    previous_son->size = (*target)->size;
    (*target)->size = getsize((*target)->left) + getsize((*target)->right) + 1;
    (*target) = previous_son;
}

void RightRotate(pTreeNode* target) {
    pTreeNode previous_son = (*target)->left;
    if (previous_son == NULL) return;
    (*target)->left = previous_son->right;
    previous_son->right = (*target);
    previous_son->size = (*target)->size;
    (*target)->size = getsize((*target)->left) + getsize((*target)->right) + 1;
    (*target) = previous_son;
}

void MaintainTree(pTreeNode* target, unsigned char flag) {
    if ((*target) == NULL) return;
    if (flag == 0) {
        if ((*target)->left == NULL) return;
        if (getsize((*target)->left->left) > getsize((*target)->right))
            RightRotate(target);
        else if (getsize((*target)->left->right) > getsize((*target)->right)) {
            LeftRotate(&((*target)->left));
            RightRotate(target);
        } else
            return;
    } else {
        if ((*target)->right == NULL) return;
        if (getsize((*target)->right->right) > getsize((*target)->left))
            LeftRotate(target);
        else if (getsize((*target)->right->left) > getsize((*target)->left)) {
            RightRotate(&((*target)->right));
            LeftRotate(target);
        } else
            return;
    }
    MaintainTree(&((*target)->left), 0);
    MaintainTree(&((*target)->right), 1);
    MaintainTree(target, 1);
    MaintainTree(target, 0);
}

void InsertNode(pTreeNode* target, void* key, PriorityCmp cmp) {
    if (*target == NULL) {
        *target = (pTreeNode)malloc(sizeof(sTreeNode));
        (*target)->left = NULL;
        (*target)->pred = NULL;
        (*target)->succ = NULL;
        (*target)->right = NULL;
        (*target)->size = 1;
        (*target)->key = key;
    } else {
        (*target)->size++;
        if (cmp(key, (*target)->key) == -1) {
            InsertNode(&((*target)->left), key, cmp);
            if ((*target)->left->size == 1) {
                (*target)->left->succ = (*target);
                (*target)->left->pred = (*target)->pred;
                if ((*target)->pred != NULL)
                    (*target)->pred->succ = (*target)->left;
                (*target)->pred = (*target)->left;
            }
            MaintainTree(target, 0);
        } else {
            InsertNode(&((*target)->right), key, cmp);
            if ((*target)->right->size == 1) {
                (*target)->right->pred = (*target);
                (*target)->right->succ = (*target)->succ;
                if ((*target)->succ != NULL)
                    (*target)->succ->pred = (*target)->right;
                (*target)->succ = (*target)->right;
            }
            MaintainTree(target, 1);
        }
    }
}

static void ReplaceByPredecessor(pTreeNode root) {
    if (root->pred->right == root) {
        root->pred->succ = root->succ;
        root->pred->right = root->right;
        if (root->succ != NULL) root->succ->pred = root->pred;
        free(root);
    } else {
        root->key = root->pred->key;
        if (root->pred->pred != NULL)
            ReplaceByPredecessor(root->pred);
        else { /*must be left of root, and it must be the head and leaf node*/
            root->pred = NULL;
            free(root->left);
            root->left = NULL;
        }
    }
}

static void ReplaceBySuccessor(pTreeNode root) {
    if (root->succ->left == root) {
        root->succ->pred = root->pred;
        root->succ->left = root->left;
        if (root->pred != NULL) root->pred->succ = root->succ;
        free(root);
    } else {
        root->key = root->succ->key;
        if (root->succ->succ != NULL)
            ReplaceBySuccessor(root->succ);
        else { /*must be right of root, and it must be the tail and leaf node*/
            root->succ = NULL;
            free(root->right);
            root->right = NULL;
        }
    }
}

static void RemoveNode(pTreeNode root, const void* key, PriorityCmp cmp) {
    while (root != NULL) {
        root->size--;

        switch (cmp(key, root->key)) {
            case 0:
                if (root->pred != NULL) /*replace by value*/
                    ReplaceByPredecessor(root);
                else
                    ReplaceBySuccessor(root);
                /*root must have at least one of pred or succ*/
                /*if not, it must be the single root of the whole tree, and
                 * should be dealt by RemoveNodeFromRoot*/
                return;
            case 1:
                root = root->right;
                break;
            case -1:
                root = root->left;
                break;
        }
    }
}

void RemoveDirectly(pTreeNode* root) {
    if ((*root)->size == 1 && (*root)->pred == NULL && (*root)->succ == NULL) {
        free(*root);
        *root = NULL;
    } else if ((*root)->pred != NULL) /*replace by value*/
        ReplaceByPredecessor(*root);
    else
        ReplaceBySuccessor(*root);
}

void RemoveNodeFromRoot(pTreeNode* root, const void* key, PriorityCmp cmp) {
    if ((*root)->size == 1) {
        if (cmp(key, (*root)->key) == 0) {
            free(*root);
            *root = NULL;
        }
    } else
        RemoveNode(*root, key, cmp);
}

pTreeNode FindTreeOfKey(pTreeNode current, const void* key, PriorityCmp cmp) {
    while (current != NULL) {
        switch (cmp(key, current->key)) {
            case 1:
                current = current->right;
                break;
            case -1:
                current = current->left;
                break;
            case 0:
                return current;
            default:
                system("pause");
        }
    }
    return NULL;
}

pTreeNode FindFirstTreeOfKey(pTreeNode current, const void* key,
                             PriorityCmp cmp) {
    while (current != NULL) {
        switch (cmp(key, current->key)) {
            case 1:
                current = current->right;
                break;
            case -1:
                current = current->left;
                break;
            case 0:
                while (current->pred != NULL)
                    if (cmp(key, current->pred->key) == 0)
                        current = current->pred;
                    else
                        break;
                return current;
            default:
                system("pause");
        }
    }
    return current;
}

pTreeNode FindNextTreeOfKey(const void* key, pTreeNode last, PriorityCmp cmp) {
    if (last->succ == NULL) return NULL;
    if (cmp(key, last->succ->key) == 0)
        return last->succ;
    else
        return NULL;
}

pTreeNode GetPredeccessorIn(pTreeNode current, const void* key,
                            PriorityCmp cmp) {
    pTreeNode last;
    last = NULL;
    while (current != NULL) {
        if (cmp(key, current->key) == 1) {
            last = current;
            current = current->right;
        } else {
            current = current->left;
        }
    }
    return last;
}

pTreeNode GetSuccessorIn(pTreeNode current, const void* key, PriorityCmp cmp) {
    pTreeNode last;
    last = NULL;
    while (current != NULL) {
        if (cmp(key, current->key) == -1) {
            last = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return last;
}
