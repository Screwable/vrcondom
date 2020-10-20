#include "Events.h"
#include "..\Helpers.h"
#include "..\IL2Cpp.h"

triggerevent_fn original_triggerevent = NULL;
int trigger_event(uintptr_t _this, uintptr_t player, uintptr_t vrc_event, uint32_t broadcast_type, uint32_t instigator_id)
{
	DWORD event_type = *(DWORD*)(vrc_event + 24); //VRC_EventHandler.VrcEventType (vrc_event + 0x18/24)
	std::string parameter_string = il2cpp_string_chars_to_string(*(const char**)(vrc_event + 32)); //ParameterString (vrc_event + 0x20/32)
	DWORD parameter_boolop = *(DWORD*)(vrc_event + 40); //VRC_EventHandler.VrcEventType (vrc_event + 0x28/40)
	
	//anti master dc
	if (parameter_string != "empty string")
	{
		//14 == sendrpc || 19 == addhealth
		if ((event_type == 14 || event_type == 19) && (parameter_string.length() > 75 || parameter_string.find("color") != std::string::npos))
			return NULL;
	}
	
	//anti world triggers && anti udon exploits B)
	if (broadcast_type == 0 || broadcast_type == 4 || broadcast_type == 7)
	{
		//10 == SetGameObjectActive || 2 == AnimationBool || Op ? 2 == toggle 
		if (event_type == 10 || event_type == 2 || parameter_boolop == 2)
			return NULL;
		
		if (event_type == 34 || parameter_string.find("UdonSyncRunProgramAsRPC") != std::string::npos || parameter_string.find("InteractWithStationRPC") != std::string::npos)
			return NULL;

	}
	
	return original_triggerevent(_this, player, vrc_event, broadcast_type, instigator_id);
}

oneventphoton_fn original_onevent_photon = NULL;
int on_event_photon(uintptr_t event_data)
{
	BYTE event_code = *(BYTE*)(event_data + 16);
	
	//anti ownership desync
	if (event_code == 210 || event_code == 209) //210 and 209 are ownership_request and ownership_transfer, iirc
		return NULL;

	return original_onevent_photon(event_data);
}

void initialize_events()
{
	original_triggerevent = triggerevent_fn(base + 0x174AEE0);
	hook_function((PVOID*)(&original_triggerevent), trigger_event);

	original_onevent_photon = oneventphoton_fn(base + 0x16D95B0);
	hook_function((PVOID*)(&original_onevent_photon), on_event_photon);
}
