/*#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>*/
#include <stdlib.h>
#include <string.h>

#include "index_tree.h"
#include "multi_index.h"
#include "time_functionality.h"

int BookingDataBlockCompareID(const char* a, const char* b) {
    return strcmp(a, b);
}
int BookingDataBlockCompareRoom(const char* a, const char* b) {
    return strcmp(a, b);
}

DEFINE_INDEX_CMP(BookingDataBlockCompareID, 1)
DEFINE_INDEX_CMP(BookingDataBlockCompareRoom, 2)

pDataBlockContainer dbc_constructor() {
    pDataBlockContainer buffer;
    buffer = CreateNewDataBlockContainer(3);
    buffer->item[0] = (char*)malloc(MAXIMUM_CHAR_LENGTH(10));
    buffer->item[1] = (char*)malloc(MAXIMUM_CHAR_LENGTH(10));
    buffer->item[2] = (char*)malloc(MAXIMUM_CHAR_LENGTH(3));
    return buffer;
}

void example_initalize_cache() {
    pMultiIndexCache db;
    pDataBlockContainer buffer;
    pDataBlockContainer searched;
    int i;
    int j;
    char roomname[4];
    db = CreateNewCache("D:/Codes/GroupData/data.dat", 3);
    roomname[3] = '\0';
    for (i = 0; i < 5; i++) {
        roomname[0] = i + '1';
        for (j = 0; j < 10; j++) {
            roomname[1] = (j + 1) / 10 + '0';
            roomname[2] = (j + 1) % 10 + '0';
            CreateNewIndex(db, roomname, 2, 1,
                           GENERATED_INDEX_CMP_LIST(BookingDataBlockCompareID));
        }
    }
    CreateNewIndex(db, "wkttud", 0, 1,
                   GENERATED_INDEX_CMP_LIST(BookingDataBlockCompareID));
    CreateNewIndex(db, "wkttud", 0, 2,
                   GENERATED_INDEX_CMP_LIST(BookingDataBlockCompareRoom));
    CreateNewIndex(db, NULL, -1, 1,
                   GENERATED_INDEX_CMP_LIST(BookingDataBlockCompareID));
    /*FILE* db_file = fopen("D:/Codes/GroupData/data.dat", "r");
    do {
        buffer = CreateNewDataBlockContainer(3);
        buffer->item[0] = (char*)malloc(MAXIMUM_CHAR_LENGTH(10));
        buffer->item[1] = (char*)malloc(MAXIMUM_CHAR_LENGTH(10));
        buffer->item[2] = (char*)malloc(MAXIMUM_CHAR_LENGTH(3));
        if (data_parse_stream(db_file, buffer) == DATA_BROKEN) {
        }
        UploadNewDataBlockToCache(db, buffer);
    } while (!feof(db_file));
    fclose(db_file);*/
    InitializeCache(db, dbc_constructor);
    /*buffer = CreateNewDataBlockContainer(3);
    buffer->item[0] = (char*)malloc(MAXIMUM_CHAR_LENGTH(10));
    buffer->item[1] = (char*)malloc(MAXIMUM_CHAR_LENGTH(10));
    buffer->item[2] = (char*)malloc(MAXIMUM_CHAR_LENGTH(3));

    searched = SearchDataBlockInCache(db, 0, "wkttud", 1, "dszz5gxx7j");
    searched = SearchDataBlockInCache(db, 0, "wkttud", 2, "209");
    searched = SearchDataBlockInCache(db, 2, "209", 1, "dszz5gxx7j");
    strcpy(buffer->item[0], searched->item[0]);
    strcpy(buffer->item[1], searched->item[1]);
    strcpy(buffer->item[2], "409");
    UpdateDataBlockInCache(db, buffer, searched, 1<<2);
    searched = SearchDataBlockInCache(db, 0, "wkttud", 1, "dszz5gxx7j");
    searched = SearchDataBlockInCache(db, 2, "209", 1, "dszz5gxx7j");
    searched = SearchDataBlockInCache(db, 2, "409", 1, "dszz5gxx7j");
    searched = SearchDataBlockInCache(db, 0, "wkttud", 2, "209");
    searched = SearchDataBlockInCache(db, 0, "wkttud", 2, "409");
    RemoveDataBlockFromCache(db, searched);
    searched = SearchDataBlockInCache(db, 0, "wkttud", 2, "409");
    searched = SearchDataBlockInCache(db, 0, "wkttud", 1, "dszz5gxx7j");*/
    /* ClearDataOfIdInCache(db, 0, "wkttud");*/
    searched = SearchDataBlockInCache(db, -1, NULL, 1, "dszz5gxx7j");
    searched = SearchDataBlockInCache(db, 0, "wkttud", 1, "dszz5gxx7j");
    /*RemoveDataBlockFromCache(db, searched);*/
    ClearDataOfIdInCache(db, 0, "wkttud");
    searched = SearchDataBlockInCache(db, -1, NULL, 1, "dszz5gxx7j");
    CloseCache(db);
    return;
}
int main() {
    sDate d;
    int cnt = 1000;
    d = GetCurrentDate();
    while (cnt--) {
        printf("%s\n", GetTimeStamp());
    }

    /*example_initalize_cache();*/
    return 0;
}