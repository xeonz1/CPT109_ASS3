#include "data_struct.h"
#include <stdlib.h>
#include "string.h"
/*data block index*/
pDataBlockIndex CreateNewDataBlockIndex(int num) {
	pDataBlockIndex pt;
	pt = (pDataBlockIndex)malloc(sizeof(sDataBlockIndex));
	pt->item_number = num;
	pt->item = (char**)malloc(sizeof(char*) * num);
	return pt;
}

void DestructDataBlockIndex(pDataBlockIndex target) {
	free(target->item);
	free(target);
}

/*data block container*/
pDataBlockContainer CreateNewDataBlockContainer(int num) {
	pDataBlockContainer pt;
	pt = (pDataBlockContainer)malloc(sizeof(sDataBlockContainer));
	pt->item_number = num;
	pt->item = (char**)malloc(sizeof(char*) * num);
	pt->next = NULL;
	pt->last = NULL;
	return pt;
}


void DestructDataBlockContainer(pDataBlockContainer target) {
	/*maintain list struct*/
	if (target->last != NULL) {
		target->last->next = target->next;
	}
	if (target->next != NULL) {
		target->next->last = target->last;
	}
	/*free items*/
	while (target->item_number--)
		free(target->item[target->item_number]);
	free(target->item);
	free(target);
}

/*list operation*/
void AppendDataBlockContainer(pDataBlockContainer node, pDataBlockContainer* tail) {
	if (*tail == NULL) {
		(*tail) = node;
	}
	else {
		node->last = *tail;
		(*tail)->next = node;
		node->next = NULL;
	}
}

void ReplaceDataBlockContainer(pDataBlockContainer old, pDataBlockContainer cur) {
	/*maintain list struct*/
	cur->next = old->next;
	cur->last = old->last;
	if (old->next != NULL) {
		old->next->last = cur;
	}
	if (old->last != NULL) {
		old->last->next = cur;
	}
	/*remove the old from list*/
	old->next = NULL;
	old->last = NULL;
}
