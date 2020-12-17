#include "user_interface.h"

#include <memory.h>
#include <stdio.h>

#include "file_io.h"
#include "hotel_macros.h"
#include "input_control.h"

void interface_StartPage(pRuntimeData data) {
#define OUTPUT_NAME "StartPage"
    static char input_buffer[2];
    /*set main interface for exceptional jump back*/
    data->main_interface = interface_StartPage;
    SET_MARK();
    system("cls");
    /*print prompt*/
    puts("\t+---------------------+");
    puts("\t|        Welcome!     |");
    puts("\t|     1:  Sign up     |");
    puts("\t|     2:  Log in      |");
    puts("\t|     3:  Exit        |");
    puts("\t+---------------------+");
    /*get input*/
    INPUT_PROMPT("option");
    INPUT(input_buffer, 1, NULL);
    /*check input option*/
    switch (input_buffer[0]) {
        case '1':
            /*user chooses to register*/
            SWITCH(interface_Register);
            break;
        case '2':
            /*user chooses to login*/
            SWITCH(interface_Login);
            break;
        case '3':
            NORMAL_INFO("Farewell");
            Delay(1000);
            return;
        default:
            /*illegal input*/
            ERROR_INFO("Input should be 1, 2 or 3!");
            GOTO_MARK();
    }
#undef OUTPUT_NAME
}

void interface_Register(pRuntimeData data) {
#define OUTPUT_NAME "Register"
    sUser registered_account, authority;
    pDataBlockContainer found_user_db;
    static char input_buffer[maximum_name_length + 1];
    fpos_t begin, end;
    SET_MARK();
    /*for user name*/
    system("cls");
    NORMAL_INFO(
        "Please input user name with only letters, number and "
        "underline");
    ATTEMP(GetInput(registered_account.username, maximum_name_length,
                    PredefinedInputCheckNormal));
    if (DataCheck(USER_LIST_DIR) == DATA_PASSED) {
        switch (DataBlockLocate(USER_LIST_DIR, USER_ID_INDEX,
                                registered_account.username, &begin, &end)) {
            case DATA_PASSED:
                ERROR_INFO("Account exists!");
                JUMP_TO_MARK();
            case DATA_UNOPENED:
            case DATA_BROKEN:
                ERROR_INFO("User list corrupted please check manually!")
                JUMP_TO(interface_StartPage);
            case DATA_NON_EXIST:
                break;
        }
    }
    SET_MARK();
    /*for password*/
    system("cls");
    printf("******Hello, %s******\n", registered_account.username);
    NORMAL_INFO(
        "Please input user password with only letters, "
        "number and underline.");
    ATTEMP(GetInput(registered_account.password, maximum_password_length,
                    PredefinedInputCheckNormal));
    NORMAL_INFO("Please confirm your password");
    ATTEMP(GetInput(input_buffer, maximum_password_length, NULL));
    if (!strcmp(registered_account.password, input_buffer)) {
        INPUT_PROMPT("Are you a receptionist or manager?(r/m)");
        INPUT(input_buffer, 1, NULL);
        switch (input_buffer[0]) {
            case 'r':
                registered_account.type = USER_RECEPTIONIST;
            case 'm':
                /*must be authorized by another manager(if exits)*/
                switch (DataBlockLocate(USER_LIST_DIR, USER_TYPE_INDEX,
                                        USER_MANAGER_STRING, &begin, &end)) {
                    case DATA_PASSED:
                        /*found manager*/
                        SET_MARK();
                        system("cls");
                        NORMAL_INFO("A manager is required to authorize.")
                        /*managers exist, ask to authorize*/
                        INPUT_PROMPT("manager user id");
                        INPUT(input_buffer, maximum_name_length,
                              PredefinedInputCheckNormal);
                        switch (DataBlockLocate(USER_LIST_DIR, USER_ID_INDEX,
                                                input_buffer, &begin, &end)) {
                            case DATA_UNOPENED:
                            case DATA_BROKEN:
                                ERROR_INFO("check user list status!");
                                SWITCH(interface_StartPage);
                            case DATA_NON_EXIST:
                                ERROR_INFO("user non exist!");
                                GOTO_MARK();
                            case DATA_PASSED:
                                /*input manager id exists*/
                                found_user_db = CreateUserDataBlock();
                                /*obtain manager user info*/
                                switch (DataBlockParse(
                                    USER_LIST_DIR, begin, end,
                                    found_user_db->item,
                                    found_user_db->item_number)) {
                                    case DATA_UNOPENED:
                                    case DATA_BROKEN:
                                        DestructDataBlockContainer(
                                            found_user_db);
                                        ERROR_INFO("check user list status!");
                                        SWITCH(interface_StartPage);
                                    case DATA_PASSED:
                                        UserRead(&authority, found_user_db);
                                        DestructDataBlockContainer(
                                            found_user_db);
                                        break;
                                }
                                break;
                        }
                        INPUT_PROMPT("manager user password");
                        INPUT(input_buffer, maximum_password_length,
                              PredefinedInputCheckNormal);
                        if (!strcmp(input_buffer, authority.password)) {
                            NORMAL_INFO("authorized");
                            Delay(1000);
                            break;
                        } else {
                            ERROR_INFO("password incorrect");
                            GOTO_MARK();
                        }
                    case DATA_UNOPENED:
                    case DATA_BROKEN:
                        ERROR_INFO("User list corrupted please check manually!")
                        JUMP_TO(interface_StartPage);
                    case DATA_NON_EXIST:
                        /*no manager*/
                        break;
                }
                registered_account.type = USER_MANAGER;
                break;
            default:
                ERROR_INFO("input must be r or m!");
                GOTO_MARK();
        }

        SET_MARK();

        /*ask for next step*/
        system("cls");
        NORMAL_INFO("User Registered.");
        NORMAL_INFO(
            "Do you want to directly login or go back to "
            "start?(l/s)");
        ATTEMP(GetInput(input_buffer, 1, NULL));
        switch (input_buffer[0]) {
            case 'l':
                memcpy(&(data->user), &registered_account, sizeof(sUser));
                JUMP_TO(interface_UserCenter);
            case 's':
                JUMP_TO(interface_StartPage);
            default:
                ERROR_INFO("Input must be l or s!");
                JUMP_TO_MARK();
        }
    } else {
        /*confirm failed*/
        ERROR_INFO("failed to confirm.");
        JUMP_TO_MARK();
    }
#undef OUTPUT_NAME
}

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