#include <stdlib.h>

#include "mapping_tree.h"

pMap CreateNewMap(void* id, void* val) {
    pMap new_map;
    new_map = (pMap)malloc(sizeof(struct MapObject));
    new_map->id = id;
    new_map->val = val;
    return new_map;
}

pMapContainer CreateMapContainer(PriorityCmp maintain_cmp, PriorityCmp id_cmp,
                                 ObjectDestructor id_destructor,
                                 ObjectDestructor val_destructor) {
    pMapContainer new_mapping;
    new_mapping = (pMapContainer)malloc(sizeof(sMapContainer));
    new_mapping->id_destructor = id_destructor;
    new_mapping->val_destructor = val_destructor;
    new_mapping->maintain_cmp = maintain_cmp;
    new_mapping->id_cmp = id_cmp;
    new_mapping->root = NULL;
    return new_mapping;
}

pTreeNode GetMapOf(const void* id, pMapContainer mapping) {
    return FindFirstTreeOfKey(mapping->root, id, mapping->id_cmp);
}

pTreeNode GetNextMappingOf(const void* id, pMapContainer mapping,
                           pTreeNode last) {
    return FindNextTreeOfKey(id, last, mapping->id_cmp);
}

void RemoveMap(pMapContainer mapping, pTreeNode last) {
    mapping->id_destructor(((pMap)last->key)->id);
    mapping->val_destructor(((pMap)last->key)->val);
    if (last == mapping->root) {
        mapping->root = RemoveDirectly(mapping->root);
    } else
        RemoveDirectly(last);
}

void UnloadMap(pMapContainer mapping, pTreeNode last) {
    if (last == mapping->root) {
        mapping->root = RemoveDirectly(mapping->root);
    } else
        RemoveDirectly(last);
}

void AddNewMap(void* id, void* val, pMapContainer mapping) {
    pMap new_map;
    new_map = CreateNewMap(id, val);
    InsertNodeFromRoot(&(mapping->root), new_map, mapping->maintain_cmp);
}

void DestructMapContainer(pMapContainer mapping) {
    pTreeNode it;
    if (mapping->root != NULL) {
        it = mapping->root->succ;
        /*root may be in the middle of the succ-pred list*/
        while (it != NULL) {
            mapping->id_destructor(((pMap)(it->key))->id);
            mapping->val_destructor(((pMap)(it->key))->val);
            free(it->key);
            it = it->succ;
        }
        it = mapping->root->pred;
        while (it != NULL) {
            mapping->id_destructor(((pMap)(it->key))->id);
            mapping->val_destructor(((pMap)(it->key))->val);
            free(it->key);
            it = it->pred;
        }
        /*lastly, destruct the root map*/
        mapping->id_destructor(((pMap)(mapping->root->key))->id);
        mapping->val_destructor(((pMap)(mapping->root->key))->val);
        free(mapping->root->key);
        /*destruct the whole tree*/
        DestructTree(mapping->root);
    }
    free(mapping);
}