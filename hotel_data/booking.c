#include "booking.h"
#include "index_tree.h"

int BookingDateCmp(const char* date1, const char* date2) {
    int res;
    /*3begin+3end*/
    res = strncmp(date1, date2 + date_item_count, date_item_count);
    if (res >= 0) return 1; /*[]()*/
    if (res < 0) {          /*(]*/
        res = strncmp(date1 + date_item_count, date2, date_item_count);
        if (res <= 0) return -1; /*()[]*/
        if (res > 0) {           /*([]) or ([)] or [(]) or [()]*/
            return 0;
        }
    }
}

DEFINE_INDEX_CMP(BOOKING_DATE_CMP, BOOKING_DATE_INDEX);