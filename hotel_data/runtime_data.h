#ifndef __RUNTIME_DATA__
#define __RUNTIME_DATA__

#include <setjmp.h>

#include "multi_index.h"
#include "user.h"

#include "time_functionality.h"
#include "input_control.h"


typedef struct RuntimeData {
    /*data members*/
    sUser user;
    pMultiIndexCache customer_db;
    pMultiIndexCache room_db;
    pMultiIndexCache booking_db;
    MONEY_TYPE room_class_price[4];
    /*control members*/
    void (*main_interface)(struct RuntimeData*);
    void (*next_interface)(struct RuntimeData*);
    jmp_buf scheduler;
    jmp_buf mark;
} sRuntimeData;

typedef sRuntimeData* pRuntimeData;

#define RUNTIME_DATA rt_data

#define SET_MARK() \
    { setjmp(RUNTIME_DATA->mark); }

#define GOTO_MARK() \
    { longjmp(RUNTIME_DATA->mark, 0); }

#define SET_SCEDULER(p_rt) \
    { setjmp(p_rt->scheduler); }

#define SWITCH(next)                                       \
    {                                                      \
        RUNTIME_DATA->next_interface = next; \
        longjmp(RUNTIME_DATA->scheduler, 0); \
    }

#define SET_MAIN(where) \
    { RUNTIME_DATA->main_interface = where; }

#define GOTO_MAIN()                                        \
    {                                                      \
        RUNTIME_DATA->main_interface = next; \
        longjmp(RUNTIME_DATA->scheduler, 0); \
    }

#define INPUT(dst, len, callback)                                        \
    {                                                                    \
        switch (GetInput(dst, len, callback)) {                          \
            case INPUT_PASSED: /*input passed*/                          \
                break;                                                   \
            case INPUT_NON: /*no input*/                                 \
                ERROR_INFO("NON INPUT");                                 \
                GOTO_MARK();                                             \
            case INPUT_ILLEGAL: /*input illegal*/                        \
                ERROR_INFO("INPUT ILLEGAL");                             \
                GOTO_MARK();                                             \
            case INPUT_OVERFLOW: /*input too long*/                      \
                ERROR_INFO("INPUT TOO LONG");                            \
                GOTO_MARK();                                             \
            case INPUT_COMMAND_BACK: /*input is "!", forced jump to main \
                                        interface*/                      \
                GOTO_MAIN();                                             \
        }                                                                \
    }

/**
 * @brief a group of info output macros, format specified
 * @warning the OUTPUT_NAME should be defined
 */
#define NORMAL_INFO(text) \
    { puts("[" OUTPUT_NAME "]: " text); }
/**
 * @brief print input prompt
 * @param text the content asked to input
 */
#define INPUT_PROMPT(text) \
    { printf("\n[" OUTPUT_NAME "] asking for " text ": "); }
/**
 * @brief print delayed error info
 * @param text the content to output
 */
#define ERROR_INFO(text)                          \
    {                                             \
        puts("[" OUTPUT_NAME "]: Error - " text); \
        Delay(strlen(text) * 50 + 100);           \
    }

#endif /*__RUNTIME_DATA_*/