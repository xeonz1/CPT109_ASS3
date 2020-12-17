#ifndef __INDEX_TREE__
#define __INDEX_TREE__

#include "SBT.h"
#include "data_struct.h"
#include "mapping_tree.h"

/*struct IndexTreeId*/
typedef struct IndexTreeIdObject {
    char* id;
    int id_index;
    int item_index;
}sIndexTreeId;

typedef sIndexTreeId TmpIndexTreeId;

TmpIndexTreeId CreateTmpIndexTreeId(char* id, const int id_index,
    const int item_index);

typedef struct IndexTreeIdObject* pIndexTreeId;
pIndexTreeId CreateNewIndexTreeId(const char* id, int id_index, int item_index);
void DestructIndexTreeId(void* target);


int IndexTreeIdCompare(const pIndexTreeId a, const pIndexTreeId b);

DECLARE_MAP_CMP(IndexTreeIdCompare);

#define DECLARE_INDEX_CMP(func)\
    int index_query_##func(const char* a, const pDataBlockContainer b);\
    int index_maintain_##func(const pDataBlockContainer a, const pDataBlockContainer b);

/*comparator function generator*/
#define DEFINE_INDEX_CMP(func, index)\
    int index_query_##func(const void* a, const void* b){\
        return func((char*)a, ((pDataBlockContainer)b)->item[index]);\
    }\
    int index_maintain_##func(const void* a, const void* b){\
        return func(((pDataBlockContainer)a)->item[index], ((pDataBlockContainer)b)->item[index]);\
    }
/*comparator function list*/
#define GENERATED_INDEX_CMP_LIST(func) index_maintain_##func, index_query_##func

/*struct IndexTree*/
typedef struct IndexTree {
    pTreeNode root;
    int item_index;
    PriorityCmp maintain_cmp;
    PriorityCmp query_cmp;
} sIndexTree;

typedef sIndexTree* pIndexTree;
/*objective functions*/
pIndexTree CreateIndexTree(int item_index, PriorityCmp maintain_cmp, PriorityCmp query_cmp);
void DestructIndexTree(void* root);
/*query function*/
pTreeNode IndexTreeBegin(pIndexTree index_tree);
pTreeNode FindIndexOfKeyInIndexTree(pIndexTree index_tree, const char* key);
pTreeNode FindNextIndexOfKeyInIndexTree(pIndexTree index_tree, const char* key, pTreeNode node);
/*modify operation*/
void RedirectIndexTo(pIndexTree index_tree, pDataBlockContainer _old,
                     pDataBlockContainer _new);
void LoadIndexToIndexTree(pIndexTree index_tree, pDataBlockContainer key);
void UnloadIndexFromIndexTree(pIndexTree index_tree, pDataBlockContainer key);

#define GET_INDEX(_map) ((pIndexTree)((pMap)(_map->key))->val)
#define GET_DATA(_index_tree_node) ((pDataBlockContainer)(_index_tree_node->key))

#endif