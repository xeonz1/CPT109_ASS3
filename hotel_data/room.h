#ifndef __ROOM__
#define __ROOM__

#include <string.h>

#include "data_struct.h"
#include "index_tree.h"
#include "time_functionality.h"
typedef enum {
    ROOM_PASSED = 0x00,
    ROOM_NON_EXIST = 0x01,
    ROOM_BOOKED = 0x03,
} ROOM_FLAG;

typedef char RoomID[4];

#define ROOM_2_LEVEL_STRING "**"
#define ROOM_3_LEVEL_STRING "***"
#define ROOM_4_LEVEL_STRING "****"
#define ROOM_VIP_STRING "VIP"

typedef enum {
    ROOM_2_LEVEL = 0,
    ROOM_3_LEVEL = 1,
    ROOM_4_LEVEL = 2,
    ROOM_VIP = 3
} ROOM_CLASS;

typedef struct Room {
    RoomID id;
    ROOM_CLASS level;
} sRoom;

#define ROOM_ITEM_COUNT 2
#define ROOM_ID_INDEX 0
#define ROOM_CLASS_INDEX 1

#define ROOM_ID_CMP strcmp;

DECLARE_INDEX_CMP(ROOM_ID_CMP);

typedef sRoom* pRoom;

pDataBlockContainer CreateRoomDataBlock();

ROOM_FLAG CustomerRead(pRoom room, pDataBlockContainer dt);
ROOM_FLAG CustomerWrite(pRoom room, pDataBlockContainer dt);

#endif