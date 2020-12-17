#ifndef __HOTEL_INTERFACE__
#define __HOTEL_INTERFACE__

#include "runtime_data.h"

/*manage*/
void interface_CutsomerDataBase(pRuntimeData data);
void interface_RoomClassManage(pRuntimeData data);
void interface_ViewHotelStatistics(pRuntimeData data);
/*reception*/

void interface_RegisterBooking(pRuntimeData data);
void interface_CheckinCustomer(pRuntimeData data);
void interface_EditBooking(pRuntimeData data);
void interface_CheckoutCustomer(pRuntimeData data);

#endif /* __HOTEL_INTERFACE_ */