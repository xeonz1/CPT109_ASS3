
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
void LeftRotate(pTreeNode target) {
    pTreeNode previous_son = target->right;
    target->right = previous_son->left;
    if (previous_son->left != NULL) previous_son->left->parent = target;
    previous_son->left = target;
    previous_son->parent = target->parent;
    if (target->parent != NULL) {
        if (target == target->parent->left)
            target->parent->left = previous_son;
        else
            target->parent->right = previous_son;
    }
    previous_son->size = target->size;
    target->parent = previous_son;
    target->size = getsize(target->left) + getsize(target->right) + 1;
}

void RightRotate(pTreeNode target) {
    pTreeNode previous_son = target->left;
    target->left = previous_son->right;
    if (previous_son->right != NULL) previous_son->right->parent = target;
    previous_son->right = target;
    previous_son->parent = target->parent;
    if (target->parent != NULL) {
        if (target == target->parent->right)
            target->parent->right = previous_son;
        else
            target->parent->left = previous_son;
    }
    previous_son->size = target->size;
    target->parent = previous_son;
    target->size = getsize(target->right) + getsize(target->left) + 1;
}

void MaintainTree(pTreeNode target, unsigned char flag) {
    if (target == NULL) return;
    if (flag == 0) {
        if (target->left == NULL) return;
        if (getsize(target->left->left) > getsize(target->right)) {
            RightRotate(target);
        } else if (getsize(target->left->right) > getsize(target->right)) {
            LeftRotate(target->left);
            RightRotate(target);
        } else
            return;
    } else {
        if (target->right == NULL) return;
        if (getsize(target->right->right) > getsize(target->left)) {
            LeftRotate(target);
        } else if (getsize(target->right->left) > getsize(target->left)) {
            RightRotate(target->right);
            LeftRotate(target);
        } else
            return;
    }
    /*switch to current root*/
    if (target->parent != NULL) target = target->parent;
    MaintainTree(target->left, 0);
    MaintainTree(target->right, 1);
    MaintainTree(target, 1);
    MaintainTree(target, 0);
}

void InsertNode(pTreeNode target, void* key, PriorityCmp cmp);

void InsertNodeFromRoot(pTreeNode* root, void* key, PriorityCmp cmp) {
    if (*root == NULL) {
        *root = (pTreeNode)malloc(sizeof(sTreeNode));
        (*root)->left = NULL;
        (*root)->pred = NULL;
        (*root)->succ = NULL;
        (*root)->right = NULL;
        (*root)->parent = NULL;
        (*root)->size = 1;
        (*root)->key = key;
    } else {
        InsertNode(*root, key, cmp);
        /*update root*/
        while ((*root)->parent != NULL) (*root) = (*root)->parent;
    }
}

void InsertNode(pTreeNode target, void* key, PriorityCmp cmp) {
    target->size++;
    if (cmp(key, target->key) == -1) {
        if (target->left == NULL) {
            InsertNodeFromRoot(&(target->left), key, cmp);
            target->left->parent = target;
            target->left->succ = target;
            target->left->pred = target->pred;
            if (target->pred != NULL) target->pred->succ = target->left;
            target->pred = target->left;
        } else {
            InsertNode(target->left, key, cmp);
            MaintainTree(target, 0);
        }
    } else {
        if (target->right == NULL) {
            InsertNodeFromRoot(&(target->right), key, cmp);
            target->right->parent = target;
            target->right->pred = target;
            target->right->succ = target->succ;
            if (target->succ != NULL) target->succ->pred = target->right;
            target->succ = target->right;
        } else {
            InsertNode(target->right, key, cmp);
            MaintainTree(target, 1);
        }
    }
}

pTreeNode RemoveDirectly(pTreeNode root) {
    pTreeNode target;
    /*maintain list*/
    if (root->succ != NULL) root->succ->pred = root->pred;
    if (root->pred != NULL) root->pred->succ = root->succ;
    if (root->right != NULL) {
        target = root->right;
        while (target->left != NULL) {
            target->size--;
            target = target->left;
        }
        /*maintain the replaced location*/
        if (root->left != NULL) root->left->parent = target;
        if (root->parent != NULL) {
            if (root->parent->left == root) root->parent->left = target;
            else root->parent->right = target;
        }
        target->left = root->left;
        if (target != root->right) {
            /*maintain sub successor locally*/
            if (target->right != NULL) target->right->parent = target->parent;
            target->parent->left = target->right;
            /*replaced location*/
            target->right = root->right;
            root->right->parent = target;
            target->size = getsize(root->left) + getsize(root->right) + 1;
        } else
            target->size += getsize(root->left);
        target->parent = root->parent;
    } else {
        target = root->left;
        if (root->parent != NULL) {
            if (root->parent->left == root) root->parent->left = target;
            else root->parent->right = target;
        }
        if(target != NULL) target->parent = root->parent;
    }
    free(root);
    /*return new root*/
    return target;
}

void RemoveNodeByKey(pTreeNode* root, const void* key, PriorityCmp cmp) {
    pTreeNode current;
    current = *root;
    while (current != NULL) {
        current->size--;
        switch (cmp(key, current->key)) {
            case 1:
                current = current->right;
                break;
            case -1:
                current = current->left;
                break;
            case 0:
                if (current == *root) {
                    *root = RemoveDirectly(current);
                    return;
                } else {
                    RemoveDirectly(current);
                    return;
                }
        }
    }
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
