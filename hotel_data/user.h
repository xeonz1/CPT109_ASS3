#ifndef __USER_HEADER__
#define __USER_HEADER__

#include "hotel_macros.h"
#include "time_functionality.h"
typedef enum { USER_MANAGER = 0, USER_RECEPTIONIST = 1 } USER_TYPE;

typedef struct User {
    char username[MAXIMUM_CHAR_LENGTH(maximum_name_length)];
    USER_TYPE type;
    char password[MAXIMUM_CHAR_LENGTH(maximum_password_length)];
} sUser;

#define USER_ITEM_COUNT 3
#define USER_ID_INDEX 0

typedef sUser* pUser;

#endif
