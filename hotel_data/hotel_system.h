#ifndef __HOTEL_SYSTEM__
#define __HOTEL_SYSTEM__

#include "runtime_data.h"

pRuntimeData HotelSystemInit();

void HotelSystemStart(pRuntimeData data);

void HotelSystemExit(pRuntimeData data);

#endif /*__HOTEL_SYSTEM_*/