#ifndef __BOOKING__
#define __BOOKING__

#include "customer.h"
#include "room.h"
#include "time_functionality.h"

typedef enum {
    BOOKING_PASSED = 1,
    BOOKING_NON_EXIST = 2,
    BOOKING_EXIST = 3
} BOOKING_FLAG;

typedef struct Booking {
    TimeStampType id;
    sDate date;
    CustomerIdType customer_id;
    RoomID room_id;
    ROOM_CLASS BOOKING_CLASS;
    MONEY_TYPE price;
    sDate begin;
    sDate end;
} sBooking;

#define BOOKING_ITEM_COUNT 4
#define BOOKING_ID_INDEX 0
#define BOOKING_DATE_INDEX 7

#define BOOKING_DATE_CMP BookingDateCmp

int BookingDateCmp(const char* date1, const char* date2);

DECLARE_INDEX_CMP(BOOKING_DATE_CMP);

typedef struct Booking* pBooking;
pDataBlockContainer CreateBookingDataBlock();
BOOKING_FLAG BookingRead(pBooking room, pDataBlockContainer dt);
BOOKING_FLAG BookingWrite(pBooking room, pDataBlockContainer dt);

#endif