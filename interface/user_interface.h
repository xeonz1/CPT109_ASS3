#ifndef __USER_INTERFACE__
#define __USER_INTERFACE__

#include "runtime_data.h"

/**
 * @brief interface of start page
 * 
 * @param data runtime data
 */
void interface_StartPage(pRuntimeData data);
/**
 * @brief interface of register new account
 * 
 * @param data runtime data 
 */
void interface_Register(pRuntimeData data);
/**
 * @brief interface of user login
 * 
 * @param data runtime data
 */
void interface_Login(pRuntimeData data);
/**
 * @brief interface of user center
 * 
 * @param data runtime data
 */
void interface_UserCenter(pRuntimeData data);
/**
 * @brief interface of delete user
 * 
 * @param data runtime data
 */
void interface_UserDelete(pRuntimeData data);
/**
 * @brief interface of change user password
 * 
 * @param data runtime data
 */
void interface_UserChangePassword(pRuntimeData data);
/**
 * @brief interface of user logout
 * 
 * @param data 
 */
void interface_UserLogout(pRuntimeData data);


#endif /*__USER_INTERFACE_*/