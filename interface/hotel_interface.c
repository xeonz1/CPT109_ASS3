#include "hotel_interface.h"

#include "booking.h"
#include "customer.h"
#include "hotel_macros.h"
#include "index_tree.h"
#include "multi_index.h"

void sub_interface_CustomerEditer(pRuntimeData data,
                                  pDataBlockContainer customer) {
#define OUTPUT_NAME "CustomerEditer"
    static char input_buffer[maximum_name_length + 1];
    sCustomer tmp_customer;
    pDataBlockContainer new_customer;
    int changed_flag;
    changed_flag = 0;
    CustomerRead(&tmp_customer, customer);
    while (1) {
        SET_SUB_MARK();
        /*print tmp_customer info*/
        NORMAL_INFO("Which item do you want to modify?");
        INPUT_PROMPT("item number (1, 2, 3) or 0 to finish");
        SUB_INPUT(input_buffer, 1, NULL);
        switch (input_buffer[0]) {
            case '1':
                INPUT_PROMPT("new name");
                SUB_INPUT(input_buffer, maximum_name_length,
                          PredefinedInputCheckLetter);
                strcpy(tmp_customer.name, input_buffer);
                changed_flag |= (1 << CUSTOMER_NAME_INDEX);
                break;
            case '2':
            INPUT_TEL:
                INPUT_PROMPT("new tel");
                SUB_INPUT(input_buffer, maximum_tel_length,
                          PredefinedInputCheckLetter);
                if (NULL != SearchDataBlockInCache(data->customer_db, -1, NULL,
                                                   CUSTOMER_TELEPHONE_INDEX,
                                                   input_buffer)) {
                    ERROR_INFO("The telephone number has been used.");
                    goto INPUT_TEL;
                } else {
                    strcpy(tmp_customer.telephone, input_buffer);
                    changed_flag |= (1 << CUSTOMER_TELEPHONE_INDEX);
                }
                break;
            case '3':
                INPUT_PROMPT("vip?(y or n)");
                SUB_INPUT(input_buffer, 1, PredefinedInputCheckYesOrNoOrCancel);
                switch (input_buffer[0]) {
                    case 'y':
                        tmp_customer.membership = CUSTOMER_VIP;
                        break;
                    case 'n':
                        tmp_customer.membership = CUSTOMER_NORMAL;
                        break;
                }
                break;
            case '0':
                new_customer = CreateCustomerDataBlock();
                /*modification done*/
                CustomerWrite(&tmp_customer, new_customer);
                /*update vip count*/
                if (strcmp(new_customer->item[CUSTOMER_MEMBERSHIP_INDEX],
                           customer->item[CUSTOMER_MEMBERSHIP_INDEX])) {
                    changed_flag |= (1 << CUSTOMER_MEMBERSHIP_INDEX);
                    if (tmp_customer.membership == CUSTOMER_VIP)
                        data->vip_cnt++;
                    else
                        data->vip_cnt--;
                }
                /*update*/
                UpdateDataBlockInCache(data->customer_db, new_customer,
                                       customer, changed_flag);
                return;
            default:
                ERROR_INFO("input must be 1, 2, 3 or 0");
        }
    }

#undef OUTPUT_NAME
}

pDataBlockContainer sub_interface_CustomerCreator(pRuntimeData data) {
#define OUTPUT_NAME "CustomerCreator"
    static char input_buffer[1 + 1];
    sCustomer new_customer;
    pDataBlockContainer customer;
    /*ask for customer info*/
    SET_SUB_MARK();
    system("cls");
    NORMAL_INFO("Please input name");
    INPUT_PROMPT("name(letters only)");
    SUB_INPUT(new_customer.name, maximum_name_length,
              PredefinedInputCheckLetter);
INPUT_TEL:
    INPUT_PROMPT("tel(11 numbers only)");
    SUB_INPUT(new_customer.telephone, maximum_tel_length,
              PredefinedInputCheckLetter);
    if (NULL != SearchDataBlockInCache(data->customer_db, -1, NULL,
                                       CUSTOMER_TELEPHONE_INDEX,
                                       input_buffer)) {
        ERROR_INFO("The telephone number has been used.");
        goto INPUT_TEL;
    } else
        strcpy(new_customer.name, input_buffer);
    INPUT_PROMPT("vip?(y or n)");
    SUB_INPUT(input_buffer, 1, PredefinedInputCheckYesOrCancel);
    if (input_buffer[0] == 'y')
        new_customer.membership = CUSTOMER_VIP;
    else
        new_customer.membership = CUSTOMER_NORMAL;
    /*info complete, check*/
    NORMAL_INFO("Please check the info. (y to continue; n to redo)");
    INPUT_PROMPT("option?(y or n)");
    SUB_INPUT(input_buffer, 1, PredefinedInputCheckYesOrCancel);
    if (input_buffer[0] == 'y') {
        new_customer.id = GetTimeStamp();
        customer = CreateCustomerDataBlock();
        CustomerWrite(&new_customer, customer);
        if (new_customer.membership == CUSTOMER_VIP) data->vip_cnt++;
        /*add to username db*/
        UploadNewDataBlockToCache(data->customer_db, customer);
        /*create new index*/
        CreateNewIndex(data->booking_db, new_customer.name,
                       BOOKING_CUSTOMER_INDEX, BOOKING_DATE_INDEX,
                       GENERATED_INDEX_CMP_LIST(BOOKING_DATE_CMP));
        NORMAL_INFO("You have created the customer. Press Enter to leave");
        WAIT_FOR_ENTER();
        return customer;
    } else
        GOTO_SUB_MARK();
#undef OUTPUT_NAME
}

void sub_interface_DeleteCustomer(pRuntimeData data,
                                  pDataBlockContainer customer) {
#define OUTPUT_NAME "DeleteCustomer"
    static char input_buffer[1 + 1];
    pIndexTree iterator;
    SET_SUB_MARK();
    iterator =
        GetDataOfId(data->booking_db, BOOKING_CUSTOMER_INDEX,
                    customer->item[CUSTOMER_ID_INDEX], BOOKING_DATE_INDEX);
    if (iterator->root != NULL) {
        NORMAL_INFO(
            "The customer still has booking(s), do you want to delete it and "
            "all its booking(s)?(y or n)");
        INPUT_PROMPT("(y or n)");
        SUB_INPUT(input_buffer, 1, PredefinedInputCheckYesOrCancel);
        if (input_buffer[0] == 'n') return;
        ClearDataOfIdInCache(data->booking_db, BOOKING_CUSTOMER_INDEX,
                             customer->item[CUSTOMER_ID_INDEX]);
    }
    /*delete customer*/
    if (!strcmp(customer->item[CUSTOMER_MEMBERSHIP_INDEX], CUSTOMER_VIP))
        data->vip_cnt--;
    RemoveDataBlockFromCache(data->customer_db, customer);
#undef OUTPUT_NAME
}

INPUT_FLAG interface_CustomerDataBaseInputCallBack(char* buffer) {
    int len;
    switch (buffer[0]) {
        case '1':
        case '2':
            switch (buffer[1]) {
                case ' ':
                    len = strlen(buffer + 2);
                    switch (buffer[0]) {
                        case 1:
                            if (sscanf(buffer + 2, "%[a-zA-Z]", buffer + 2))
                                if (strlen(buffer + 2) == len)
                                    return INPUT_PASSED;
                        case 2:
                            if (sscanf(buffer + 2, "%[0-9]", buffer + 2))
                                if (strlen(buffer + 2) == len)
                                    return INPUT_PASSED;
                    }
            }
        case '3':
        case '4':
        case '5':
            if (buffer[1] == '\0') return INPUT_PASSED;
        default:
            return INPUT_ILLEGAL;
    }
}

void interface_CutsomerDataBase(pRuntimeData data) {
    static char input_buffer[2 + maximum_name_length + 1];
    pDataBlockContainer customer;
    pIndexTree customer_name_index;
    pTreeNode name_index_iterator;
#define OUTPUT_NAME "CustomerDataBase"
    /*interface print*/
    SET_MARK();
    system("cls");
    /*get customer data from db*/
    customer_name_index =
        GetDataOfId(data->customer_db, -1, NULL, CUSTOMER_NAME_INDEX);
    puts("\n\n\t\tCustomer Database Management");
    puts("\t\tOption: 1 [name] search by name");
    puts("\t\tOption: 2 [id] search by id");
    puts("\t\tOption: 3 add a customer");

    INPUT_PROMPT("option");
    INPUT(input_buffer, 2 + maximum_name_length,
          interface_CustomerDataBaseInputCallBack);
    switch (input_buffer[0]) {
        case '1': /*search by name*/
            name_index_iterator = FindIndexOfKeyInIndexTree(customer_name_index,
                                                            input_buffer + 2);
        CUSTOMER_SEARCHING:
            system("cls");
            /*print customr*/
            customer = name_index_iterator->key;
            NORMAL_INFO("Is this the customer you are looking for?(y/n)");
            INPUT_PROMPT("answer");
            INPUT(input_buffer, 1, PredefinedInputCheckYesOrNoOrCancel);
            switch (input_buffer[0]) {
                case 'y':
                CUSTOMER_ACTION:
                    NORMAL_INFO(
                        "You want to edit or delete it?(y to edit/n to "
                        "delete/c to cancel)");
                    INPUT_PROMPT("answer");
                    INPUT(input_buffer, 1, NULL);
                    switch (input_buffer[0]) {
                        case 'y':
                            sub_interface_CustomerEditer(data, customer);
                            break;
                        case 'n':
                            sub_interface_DeleteCustomer(data, customer);
                            break;
                        case 'c':
                            GOTO_MARK();
                        default:
                            ERROR_INFO("option input error");
                            goto CUSTOMER_ACTION;
                    }
                    break;
                case 'n':
                    name_index_iterator = FindNextIndexOfKeyInIndexTree(
                        customer_name_index,
                        customer->item[CUSTOMER_NAME_INDEX],
                        name_index_iterator);
                    goto CUSTOMER_SEARCHING;
            }
            break;
        case '2':
            customer =
                SearchDataBlockInCache(data->customer_db, -1, NULL,
                                       CUSTOMER_ID_INDEX, input_buffer + 2);
            system("cls");
            /*print customr*/
            NORMAL_INFO("Is this the customer you are looking for?(y/n)");
            switch (input_buffer[0]) {
                case 'y':

                    sub_interface_CustomerEditer(data, customer);
                    break;
                case 'n':
                    NORMAL_INFO("Please check the id!");
                    Delay(1000);
            }
            break;
        case '3':
            sub_interface_CustomerCreator(data);
            break;
    }
    GOTO_MARK();
#undef OUTPUT_NAME
}