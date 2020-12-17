#ifndef __INPUT_CONTROL__
#define __INPUT_CONTROL__

#include <stdio.h>
/**
 * @brief enum type INPUT_FLAG given to InputHandler to deal with
 *
 */
typedef enum {
    INPUT_NON = -1,
    INPUT_PASSED = 0,
    INPUT_ILLEGAL = 1,
    INPUT_OVERFLOW = 2,
    INPUT_COMMAND_BACK = 3,
    INPUT_COMMAND_LEAVE = 4
} INPUT_FLAG;

typedef INPUT_FLAG (*InputCallback)(char* src);

#define WAIT_FOR_ENTER()          \
    {                             \
        while (getchar() != '\n') \
            ;                     \
    }

/**
 * @brief Get the Input from keyboard
 *
 * @param dst target buffer
 * @param size the maximum size of input: expect longest length of string
 * @param check the external check specified by interfaces
 */
INPUT_FLAG GetInput(char* dst, const int size, InputCallback check);

/*predefined input check*/

/**
 * @brief predefined input check: input must be [a-zA-Z1-9_]
 *
 * @param src the buffer to check
 * @return INPUT_FLAG
 */
INPUT_FLAG PredefinedInputCheckNormal(const char* src);

/**
 * @brief predefined input check: input must be [a-zA-Z]
 *
 * @param src the buffer to check
 * @return INPUT_FLAG
 */
INPUT_FLAG PredefinedInputCheckLetter(const char* src);

/**
 * @brief predefined input check: input must be [1-9]
 *
 * @param src the buffer to check
 * @return INPUT_FLAG
 */
INPUT_FLAG PredefinedInputCheckNumber(const char* src);

/**
 * @brief predefined input check: input must be Y or N or !
 *
 * @param src the buffer to check
 * @return INPUT_FLAG
 */
INPUT_FLAG PredefinedInputCheckYesOrNoOrCancel(const char* src);

/**
 * @brief predefined input check: input must be Y or !
 *
 * @param src the buffer to check
 * @return INPUT_FLAG
 */
INPUT_FLAG PredefinedInputCheckYesOrCancel(const char* src);

#endif /*__INPUT_CONTROL_*/