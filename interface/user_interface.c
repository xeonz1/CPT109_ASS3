#include "user_interface.h"

#include <memory.h>
#include <stdio.h>

#include "file_io.h"
#include "hotel_macros.h"
#include "input_control.h"
/**
 * @brief interface of user login
 *
 * @param data runtime data
 */
void interface_Login(pRuntimeData data) {
    static char input_buffer[maximum_name_length + 1];
    fpos_t begin, end;
    pDataBlockContainer found_user_db;
    sUser found_user;
#define OUTPUT_NAME "Login"
    SET_MARK();
    system("cls");
    if (DataCheck(USER_LIST_DIR) != DATA_PASSED) {
        NORMAL_INFO("No user.");
        NORMAL_INFO("Input s to go back, r to create a new account.");
        INPUT_PROMPT("option");
        ATTEMP(GetInput(input_buffer, 1, NULL));
        switch (input_buffer[0]) {
            case 's':
                SWITCH(interface_StartPage);
            case 'r':
                SWITCH(interface_Register);
            default:
                ERROR_INFO("input must be s or r!");
                GOTO_MARK();
        }
    }
    INPUT_PROMPT("user id");
    INPUT(input_buffer, maximum_name_length, PredefinedInputCheckNormal);
    switch (DataBlockLocate(USER_LIST_DIR, USER_ID_INDEX, input_buffer, &begin,
                            &end)) {
        case DATA_UNOPENED:
        case DATA_BROKEN:
            ERROR_INFO("check user list status!");
            SWITCH(interface_StartPage);
        case DATA_NON_EXIST:
            ERROR_INFO("user non exist!");
            GOTO_MARK();
        case DATA_PASSED:
            found_user_db = CreateUserDataBlock();
            /*obtain user info*/
            switch (DataBlockParse(USER_LIST_DIR, begin, end,
                                   found_user_db->item,
                                   found_user_db->item_number)) {
                case DATA_UNOPENED:
                case DATA_BROKEN:
                    DestructDataBlockContainer(found_user_db);
                    ERROR_INFO("check user list status!");
                    SWITCH(interface_StartPage);
                case DATA_PASSED:
                    UserRead(&found_user, found_user_db);
                    DestructDataBlockContainer(found_user_db);
                    break;
            }
            break;
    }
    SET_MARK();
    system("cls");
    printf("\n\t\tWelcome, User ID %s", found_user.username);
    INPUT_PROMPT("user password");
    INPUT(input_buffer, maximum_password_length, PredefinedInputCheckNormal);
    if (!strcmp(input_buffer, found_user.password)) {
        NORMAL_INFO("Login");
        memcpy(&data->user, &found_user, sizeof(sUser));
        Delay(1000);
        SWITCH(interface_UserCenter);
    } else {
        ERROR_INFO("password incorrect");
        GOTO_MARK();
    }
#undef OUTPUT_NAME
}