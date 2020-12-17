#ifndef __TIME_FUNCTIONALITY__
#define __TIME_FUNCTIONALITY__

#include <time.h>

#define time_stamp_string_length 12
#define date_item_count 3
#define date_string_length (2 + 1 + 3 + 1 + 4)

typedef struct Date {
    char year;
    char month;
    char day;
} sDate;

typedef time_t TimeStampType;

sDate GetDate(char year, char month, char day);

sDate GetCurrentDate();

char* ConvertDateToString(sDate* date);

TimeStampType GetTimeStamp();

double DiffDate(sDate* a, sDate* b);

void Delay(int milliseconds);

#endif /*__TIME_FUNCTIONALITY_*/