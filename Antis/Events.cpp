#include "Events.h"
#include "..\Helpers.h"
#include "..\IL2Cpp.h"
#include "..\Addresses.h"
#include "..\Settings.h"

triggerevent_fn original_triggerevent = NULL;
int trigger_event(uintptr_t _this, uintptr_t player, uintptr_t vrc_event, uint32_t broadcast_type, uint32_t instigator_id)
{
	if (player != 0)
	{
		if (is_local_player(player))
			return original_triggerevent(_this, player, vrc_event, broadcast_type, instigator_id);

		if (is_friend(player) && allow_friends)
			return original_triggerevent(_this, player, vrc_event, broadcast_type, instigator_id);
	}

	DWORD event_type = *(DWORD*)(vrc_event + 24); //VRC_EventHandler.VrcEventType (vrc_event + 0x18/24)
	std::string parameter_string = il2cpp_string_chars_to_string(*(const char**)(vrc_event + 32)); //ParameterString (vrc_event + 0x20/32)
	DWORD parameter_boolop = *(DWORD*)(vrc_event + 40); //VRC_EventHandler.VrcEventType (vrc_event + 0x28/40)
	uintptr_t parameter_object = *(uintptr_t*)(vrc_event + 56); //ParameterObject (vrc_event + 0x38/56)

	//anti master dc
	//14 == sendrpc || 19 == addhealth
	if (anti_master_dc && ((event_type == 14 || event_type == 19) && (parameter_string.length() > 75 || parameter_string.find("color") != std::string::npos)))
	{
		if (extensive_logging)
			printf("\nANTI MASTER DC - %u SENT CODE %u\n", instigator_id, event_type);
		return NULL;
	}

	if (broadcast_type == 0 || broadcast_type == 4 || broadcast_type == 7)
	{
		//anti world triggers
		//10 == SetGameObjectActive || 2 == AnimationBool || Op ? 2 == toggle 
		if (anti_world_triggers && (event_type == 10 || event_type == 2 || parameter_boolop == 2))
		{
			if (extensive_logging)
				printf("\nANTI WORLD TRIGGERS - %u SENT CODE %u\n", instigator_id, event_type);
			return NULL;
		}

		//anti udon events
		if (anti_udon_events && (event_type == 34 || parameter_string.find("UdonSyncRunProgramAsRPC") != std::string::npos))
		{
			if (extensive_logging)
				printf("\nANTI UDON - %u SENT CODE %u\n", instigator_id, event_type);
			return NULL;
		}
	}

	//anti camera freeze
	if (anti_camera_freeze && event_type == 14)
	{
		if ((parameter_string != "SpawnEmojiRPC" && parameter_string != "PlayEmoteRPC" && parameter_string != "TeleportRPC" && parameter_string != "InteractWithStationRPC") && (get_object_name(parameter_object) == "Indicator" || get_object_name(parameter_object).find("VRCPlayer[Remote]") != std::string::npos))
		{
			if (extensive_logging)
				printf("\nANTI CAMERA FREEZE [2] - %u SENT CODE %u\n", instigator_id, event_type);
			return NULL;
		}

		//if something messes up, lets just introduce an extra measure for safety purposes ^^
		if (parameter_string == "PhotoCapture" || parameter_string == "ChangeVisibility" || parameter_string == "TimerBloop")
		{
			if (extensive_logging)
				printf("\nANTI CAMERA FREEZE [1] - %u SENT CODE %u\n", instigator_id, event_type);
			return NULL;
		}
	}

	return original_triggerevent(_this, player, vrc_event, broadcast_type, instigator_id);
}

oneventphoton_fn original_onevent_photon = NULL;
int on_event_photon(uintptr_t event_data)
{
	BYTE event_code = *(BYTE*)(event_data + 16);
	uint32_t sender = *(uint32_t*)(event_data + 32);
	uintptr_t player = get_player_from_instigator(sender);

	if (player != 0)
	{
		if (is_local_player(player))
			return original_onevent_photon(event_data);

		if (is_friend(player) && allow_friends)
			return original_onevent_photon(event_data);
	}

	//anti ownership desync
	if (anti_ownership_desync && (event_code == 210 || event_code == 209)) //210 and 209 are ownership_request and ownership_transfer
	{
		if (extensive_logging)
			printf("\nANTI OWNERSHIP DESYNC - %u SENT CODE %u\n", sender, event_code);
		return NULL;
	}

	return original_onevent_photon(event_data);
}

void initialize_events()
{
	original_triggerevent = triggerevent_fn(base + triggerevent);
	hook_function((PVOID*)(&original_triggerevent), trigger_event);

	original_onevent_photon = oneventphoton_fn(base + oneventphoton);
	hook_function((PVOID*)(&original_onevent_photon), on_event_photon);
}
