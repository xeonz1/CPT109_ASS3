#ifndef __CUSTORMER__
#define __CUSTORMER__

#include "data_struct.h"
#include "hotel_macros.h"
#include "index_tree.h"
#include "time_functionality.h"

typedef enum {
    CUSTOMER_PASSED = 0x00,
    CUSTOMER_NON_EXIST = 0x01,
    CUSTOMER_DATA_CORRUPTED = 0x02,
    CUSTOMER_EXIST = 0x03
} CUSTOMER_FLAG;

typedef TimeStampType CustomerIdType;

#define CUSTOMER_ID_CMP strcmp

DECLARE_INDEX_CMP(CUSTOMER_ID_CMP);

#define CUSTOMER_VIP 1
#define CUSTOMER_NORMAL 2

typedef struct Customer {
    CustomerIdType id;
    char name[MAXIMUM_CHAR_LENGTH(maximum_name_length)];
    char telephone[MAXIMUM_CHAR_LENGTH(maximum_tel_length)];
    int membership;
    int booking_count;
} sCustomer;

#define CUSTOMER_ID_INDEX 0
#define CUSTOMER_NAME_INDEX 1
#define CUSTOMER_TELEPHONE_INDEX 2
#define CUSTOMER_MEMBERSHIP 3
#define CUSTOMER_BOOKING_COUNT_INDEX 4
#define CUSTOMER_ITEM_COUNT 5
#define CUSTOMER_NAME_CMP strcmp
#define CUSTOMER_TELEPHONE_CMP strcmp

DECLARE_INDEX_CMP(CUSTOMER_NAME_CMP);
DECLARE_INDEX_CMP(CUSTOMER_TELEPHONE_CMP);

typedef sCustomer* pCustomer;

pDataBlockContainer CreateCustomerDataBlock();

CUSTOMER_FLAG CustomerRead(pCustomer customer, pDataBlockContainer dt);
CUSTOMER_FLAG CustomerWrite(pCustomer customer, pDataBlockContainer dt);

#endif /*__CUSTOMER__*/