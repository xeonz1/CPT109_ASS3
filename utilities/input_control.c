#include "input_control.h"
#include <string.h>
INPUT_FLAG GetInput(char* dst, const int size, InputCallback check) {
    register char ch;
    register int cnt;
    INPUT_FLAG flag;
    /*initialize input character count*/
    cnt = 0;
    /*control input length*/
    while (cnt <= size) {
        ch = getchar();
        if (ch == '\n') {    /*end character of an input*/
            dst[cnt] = '\0'; /*add terminating character*/
            break;
        } else
            dst[cnt++] = ch; /*store input charcters*/
    }
    if (cnt) {
        /*if any character input*/
        if (ch == '\n') {
            /*end within limited size*/
            /*do the check*/
            if (dst[0] == '!' &&
                dst[1] == '\0') /*forced jump to main interface check*/
                return INPUT_COMMAND_BACK;
            else if (check != NULL)
                /*if external checker specified, use it to check*/
                return check(dst);
            else
                /*otherwise, input is passed (since no more restriction)*/
                return INPUT_PASSED;
        } else {
            /*not end normally = input too long, clean stdin*/
            while (getchar() != '\n')
                ;
            return INPUT_OVERFLOW;
        }
    } else /*no input get!*/
        return INPUT_NON;
}

INPUT_FLAG PredefinedInputCheckNormal(const char* src) {
    int cnt;
    cnt = strlen(src);
    /*filter all input to be [a-zA-Z0-9_]*/
    if (sscanf(src, "%[a-zA-Z0-9_]", src) != 1 || strlen(src) != cnt)
        return INPUT_ILLEGAL;
    else
        return INPUT_PASSED;
}

INPUT_FLAG PredefinedInputCheckNumber(const char* src) {
    int cnt;
    cnt = strlen(src);
    /*filter all input to be [0-9]*/
    if (sscanf(src, "%[0-9]", src) != 1 || strlen(src) != cnt)
        return INPUT_ILLEGAL;
    else
        return INPUT_PASSED;
}

INPUT_FLAG PredefinedInputCheckYesOrNoOrCancel(const char* src) {
    if (src[0] == 'Y' || src[0] == 'N') {
        if (src[1] == '\0') return INPUT_PASSED;
    } else /*why illegal instead of checking "!"? the detection of "!" is prior
              to external checker, it must have been checked*/
        return INPUT_ILLEGAL;
}

INPUT_FLAG PredefinedInputCheckYesOrCancel(const char* src) {
    if (src[0] == 'Y') {
        if (src[1] == '\0') return INPUT_PASSED;
    } else /*why illegal instead of checking "!"? the detection of "!" is prior
               to external checker, it must have been checked*/
        return INPUT_ILLEGAL;
}
