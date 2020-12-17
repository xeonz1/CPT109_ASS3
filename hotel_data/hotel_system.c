#include "hotel_system.h"

#include "booking.h"
#include "customer.h"
#include "hotel_macros.h"
#include "room.h"
#include "user_interface.h"

pRuntimeData HotelSystemInit() {
    FILE* price;
    pRuntimeData data;
    pTreeNode iterator;
    pDataBlockContainer list;
    data = (pRuntimeData)malloc(sizeof(sRuntimeData));
    /*customer database and index*/
    data->customer_db = CreateNewCache(CUSTOMER_LIST_DIR, CUSTOMER_ITEM_COUNT);
    CreateNewIndex(data->customer_db, NULL, -1, CUSTOMER_ID_INDEX,
                   GENERATED_INDEX_CMP_LIST(CUSTOMER_ID_CMP));
    CreateNewIndex(data->customer_db, NULL, -1, CUSTOMER_NAME_INDEX,
                   GENERATED_INDEX_CMP_LIST(CUSTOMER_NAME_CMP));
    CreateNewIndex(data->customer_db, NULL, -1, CUSTOMER_TELEPHONE_INDEX,
                   GENERATED_INDEX_CMP_LIST(CUSTOMER_TELEPHONE_CMP));
    InitializeCache(data->customer_db, CreateCustomerDataBlock);
    /*room database and index*/
    data->room_db = CreateNewCache(ROOM_LIST, ROOM_ITEM_COUNT);
    CreateNewIndex(data->room_db, NULL, -1, ROOM_ID_INDEX,
                   GENERATED_INDEX_CMP_LIST(ROOM_ID_CMP));
    InitializeCache(data->room_db, CreateRoomDataBlock);
    /*booking database and index*/
    data->booking_db = CreateNewCache(BOOKING_DB, BOOKING_ITEM_COUNT);
    /*establish database for each room*/
    list = data->room_db->data_list_tail;
    while (list != NULL) {
        CreateNewIndex(data->booking_db, list->item[ROOM_ID_INDEX],
                       ROOM_ID_INDEX, BOOKING_DATE_INDEX,
                       GENERATED_INDEX_CMP_LIST(BOOKING_DATE_CMP));
        list = list->last;
    }
    list = data->customer_db->data_list_tail;
    while (list != NULL) {
        CreateNewIndex(data->booking_db, list->item[CUSTOMER_ID_INDEX],
                       CUSTOMER_ID_INDEX, BOOKING_DATE_INDEX,
                       GENERATED_INDEX_CMP_LIST(BOOKING_DATE_CMP));
        list = list->last;
    }
    InitializeCache(data->booking_db, CreateBookingDataBlock);
    /*room class price*/
    price = fopen(PRICE_LIST, "r");
    fscanf(price, "%f,%f,%f,%f", data->room_class_price[0],
           data->room_class_price[1], data->room_class_price[2],
           data->room_class_price[3]);
    fclose(price);
    /*runtime config*/
    data->next_interface = interface_StartPage;
    return data;
}

void HotelSystemStart(pRuntimeData data) {
    SET_SCEDULER(data);
    data->next_interface(data);
    HotelSystemExit(data);
    return;
}

void HotelSystemExit(pRuntimeData data) {
    FILE* price;
    CloseCache(data->customer_db);
    CloseCache(data->room_db);
    CloseCache(data->booking_db);
    price = fopen(PRICE_LIST, "w");
    fprintf(price, "%f,%f,%f,%f", data->room_class_price[0],
            data->room_class_price[1], data->room_class_price[2],
            data->room_class_price[3]);
    fclose(price);
    free(data);
}