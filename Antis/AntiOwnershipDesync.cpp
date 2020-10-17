#include "AntiOwnershipDesync.h"
#include "..\Helpers.h"

oneventphoton_fn original_onevent_photon = NULL;
int on_event_photon(uintptr_t event_data)
{
	BYTE event_code = *(BYTE*)(event_data + 16);
	if (event_code == 210 || event_code == 209) //210 and 209 are ownership_request and ownership_transfer, iirc
		return NULL;

	return original_onevent_photon(event_data);
}

void initialize_anti_ownership_desync()
{
	original_onevent_photon = oneventphoton_fn(base + 0x16D95B0);
	hook_function((PVOID*)(&original_onevent_photon), on_event_photon);
}