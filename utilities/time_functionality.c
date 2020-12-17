#include "time_functionality.h"

sDate GetDate(char year, char month, char day) {
    sDate new_date;
    new_date.day = day;
    new_date.month = month;
    new_date.year = year;
    return new_date;
}

sDate GetCurrentDate() {
    sDate new_date;
    static time_t _Time;
    struct tm* timeinfo;
    time(&_Time);
    timeinfo = localtime(&_Time);
    new_date.day = timeinfo->tm_mday;
    new_date.month = timeinfo->tm_mon;
    new_date.year = timeinfo->tm_year;
    return new_date;
}

char* ConvertDateToString(sDate* date) {
    static char date_str[date_string_length + 1];
    static const char mon_name[][4] = { "Jan", "Feb", "Mar", "Apr",
                                       "May", "Jun", "Jul", "Aug",
                                       "Sep", "Oct", "Nov", "Dec" };
    sprintf(date_str, "%d %s %d", date->year + 1900, mon_name[date->month],
        date->day);
    return date_str;
}

TimeStampType GetTimeStamp() {
    static time_t _Time;
    time(&_Time);
    return _Time;
}

double DiffDate(sDate* a, sDate* b) {
    struct tm timeinfo_a, timeinfo_b;
    timeinfo_a.tm_sec = 0;
    timeinfo_a.tm_hour = 0;
    timeinfo_a.tm_min = 0;
    timeinfo_a.tm_mday = a->day;
    timeinfo_a.tm_mon = a->month;
    timeinfo_a.tm_year = a->year;

    timeinfo_b.tm_sec = 0;
    timeinfo_b.tm_hour = 0;
    timeinfo_b.tm_min = 0;
    timeinfo_b.tm_mday = b->day;
    timeinfo_b.tm_mon = b->month;
    timeinfo_b.tm_year = b->year;

    return difftime(mktime(&timeinfo_a), mktime(&timeinfo_b)) / 60 / 60 / 24;
}

void Delay(int milliseconds) {
    long pause;
    clock_t now, then;
    pause = milliseconds * (CLOCKS_PER_SEC / 1000);
    now = then = clock();
    while ((now - then) < pause) now = clock();
}
