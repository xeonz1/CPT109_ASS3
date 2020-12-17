#include <stdlib.h>
#include <string.h>

#include "index_tree.h"

TmpIndexTreeId CreateTmpIndexTreeId(char* id, const int id_index,
                                    const int item_index) {
    TmpIndexTreeId new_id;
    new_id.id = id;
    new_id.id_index = id_index;
    new_id.item_index = item_index;
    return new_id;
}

int IndexTreeIdCompare(const pIndexTreeId a, const pIndexTreeId b) {
    if (a->id_index == b->id_index) {
        if (a->id != NULL && b->id != NULL) {
            if (!strcmp(a->id, b->id)) {
                if (a->item_index == -1)
                    return 0;
                else
                    return (a->item_index == b->item_index)
                               ? 0
                               : ((a->item_index > b->item_index) ? 1 : -1);
            } else
                return strcmp(a->id, b->id);
        } else {
            return (a->item_index == b->item_index)
                       ? 0
                       : ((a->item_index > b->item_index) ? 1 : -1);
        }
    } else
        return a->id_index > b->id_index ? 1 : -1;
}
DEFINE_MAP_CMP(IndexTreeIdCompare, sIndexTreeId)

pIndexTreeId CreateNewIndexTreeId(const char* id, int id_index,
                                  int item_index) {
    pIndexTreeId new_id;
    new_id = (pIndexTreeId)malloc(sizeof(sIndexTreeId));

    new_id->id_index = id_index;
    new_id->item_index = item_index;
    if (id != NULL) {
        new_id->id = (char*)malloc(MAXIMUM_CHAR_LENGTH(strlen(id)));
        strcpy(new_id->id, id);
    } else
        new_id->id = NULL;
    return new_id;
}

void DestructIndexTreeId(void* target) {
    if (((pIndexTreeId)target)->id != NULL) free(((pIndexTreeId)target)->id);
    free(((pIndexTreeId)target));
}