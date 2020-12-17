#ifndef __DATA_STRUCT__
#define __DATA_STRUCT__
#include "stdio.h"


#define MAXIMUM_CHAR_LENGTH(val) (sizeof(char)*val + 1)


/*struct of the pos of a block in a data file*/
typedef struct {
    fpos_t begin;
    fpos_t end;
} data_block_pos;

typedef struct DataBlockIndex {
    char** item;
    int item_number;
} sDataBlockIndex;

typedef sDataBlockIndex* pDataBlockIndex;

pDataBlockIndex CreateNewDataBlockIndex(int num);
void DestructDataBlockIndex(pDataBlockIndex target);

typedef struct DataBlockContainer {
    char** item;
    int item_number;
    struct DataBlockContainer* next;
    struct DataBlockContainer* last;
} sDataBlockContainer;

typedef sDataBlockContainer* pDataBlockContainer;
/**
 * @brief specialized constructor, must alloc all the space needed by object
 * 
 */
typedef pDataBlockContainer (*ConstructDataBlockContainer)(void);

pDataBlockContainer CreateNewDataBlockContainer(int number_of_items);

void ReplaceDataBlockContainer(pDataBlockContainer old, pDataBlockContainer cur);

void DestructDataBlockContainer(pDataBlockContainer target);

void AppendDataBlockContainer(pDataBlockContainer node, pDataBlockContainer* tail);

#endif