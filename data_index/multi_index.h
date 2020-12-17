#ifndef __MULTI_INDEX__
#define __MULTI_INDEX__

#include "data_struct.h"
#include "mapping_tree.h"

typedef struct MultiIndexCache {
    int data_item_size;
    char* dir;
    char* log_dir;
    FILE* log;
    int log_enable;
    pMapContainer id_to_index_tree_mapping;
    pDataBlockContainer data_list_tail;
} sMultiIndexCache;

typedef sMultiIndexCache* pMultiIndexCache;

void CacheWriteLog(pMultiIndexCache db, char op, ...);

void UploadNewDataBlockToCache(pMultiIndexCache db, pDataBlockContainer key);
void RemoveDataBlockFromCache(pMultiIndexCache db, pDataBlockContainer key);
pDataBlockContainer SearchDataBlockInCache(pMultiIndexCache db,
                                           const int id_index, char* id,
                                           const int item_index,
                                           const char* key);
void UpdateDataBlockInCache(pMultiIndexCache db, pDataBlockContainer new_key,
                            pDataBlockContainer old_key, int changed_index);
pMultiIndexCache CreateNewCache(const char* dir, int item_number);
void InitializeCache(pMultiIndexCache db,
                     ConstructDataBlockContainer constructor);
void CloseCache(pMultiIndexCache db);

void CreateNewIndex(pMultiIndexCache db, char* id, int id_index, int item_index,
                    PriorityCmp maintain_cmp, PriorityCmp query_cmp);

pTreeNode GetDataOfId(pMultiIndexCache db, const int id_index, char* id, int ordered_item_index);
void ClearDataOfIdInCache(pMultiIndexCache db, int id, char* text);

void example_initalize_cache();
#endif