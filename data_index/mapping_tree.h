#ifndef __MAPPING_TREE__
#define __MAPPING_TREE__

#include "SBT.h"

typedef struct MapObject {
    void* id;
    void* val;
} * pMap;

#define DECLARE_MAP_CMP(func)                                                \
    int map_query_##func(const void* compared_id, const void* compared_map); \
    int map_maintain_##func(const void* a, const void* b)

#define DEFINE_MAP_CMP(func, type)                                            \
    int map_query_##func(const void* compared_id, const void* compared_map) { \
        return func((type*)compared_id,                                 \
                    (type*)(((pMap)compared_map)->id));            \
    }                                                                         \
    int map_maintain_##func(const void* a, const void* b) {                   \
        return func((type*)(((pMap)a)->id),                       \
                    (type*)(((pMap)b)->id));                      \
    }

#define GENERATED_MAP_CMP_LIST(func) map_maintain_##func, map_query_##func

typedef void (*ObjectDestructor)(void* obj);

typedef struct MapContainer {
    pTreeNode root;
    PriorityCmp maintain_cmp; /*argument: pMap, pMap*/
    PriorityCmp id_cmp;       /*argument: id type, pMap*/
    ObjectDestructor id_destructor;
    ObjectDestructor val_destructor;
} sMapContainer;

typedef sMapContainer* pMapContainer;

pMapContainer CreateMapContainer(PriorityCmp maintain_cmp, PriorityCmp id_cmp,
                                 ObjectDestructor id_destructor,
                                 ObjectDestructor val_destructor);
void DestructMapContainer(pMapContainer mapping);

pTreeNode GetMapOf(const void* id, pMapContainer mapping);
pTreeNode GetNextMappingOf(const void* id, pMapContainer mapping,
                           pTreeNode last);
void RemoveMap(pMapContainer mapping, pTreeNode last);
void UnloadMap(pMapContainer mapping, pTreeNode last);
void AddNewMap(void* id, void* val, pMapContainer mapping);

#endif