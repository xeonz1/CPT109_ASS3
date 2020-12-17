#ifndef __HOTEL_MACROS__
#define __HOTEL_MACROS__

#define maximum_password_length     (63)
#define maximum_name_length         (63)
#define maximum_id_length           (12)
#define maximum_tel_length          (11)
#define maximum_membership_length   (12)
#define maximum_count_length        (6)
#define MAXIMUM_CHAR_LENGTH(val)    (val+1)
#define maximum_price_length         (9)

#define hotel_floors            5
#define rooms_per_floor         10
#define hotel_rooms             (hotel_floors*rooms_per_floor)
#define classes_of_rooms        4
#define customer_per_page       8
#define maximum_price           9999999
#define minimum_price           0.1

#define DIR_PREFIX          D:/CPT109 CODE/Group
#define STR_MACRO(s)        #s
#define USER_LIST_DIR       STR_MACRO(DIR_PREFIX/USER_LIST.dat)
#define ROOM_LIST           STR_MACRO(DIR_PREFIX/ROOM_LIST.dat)
#define ROOM_LIST           STR_MACRO(DIR_PREFIX/ROOM_PRICE.dat)
#define CUSTOMER_LIST_DIR   STR_MACRO(DIR_PREFIX/CUSTOMER_LIST.dat)
#define STATISTICS_DIR      STR_MACRO(DIR_PREFIX/STATISTICS.dat)
#define BOOKING_DB       STR_MACRO(DIR_PREFIX/BOOKING_DB.dat)
#define PRICE_LIST STR_MACRO(DIR_PREFIX/PRICE_LIST.dat)

typedef float MONEY_TYPE;

#endif