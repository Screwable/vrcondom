#include "Helpers.h"
#include "Detours\detours.h"
#include "Addresses.h"

void hook_function(PVOID* func, PVOID detour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(func, detour);
	DetourTransactionCommit();
}

std::string utf16_to_utf8(std::u16string str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	const int16_t* i = reinterpret_cast<const int16_t*>(str.data());
	return convert.to_bytes(i, i + str.size());
}

bool is_local_player(uintptr_t player)
{
	//ida psuedocode from islocal function
	uintptr_t result = *(uintptr_t*)(player + 48);
	if (result)
		result = *(BYTE*)(result + 36);

	return result;
}

uintptr_t get_player_from_instigator(uint32_t id)
{
	using fn = uintptr_t(*)(uint32_t);
	fn function = (fn)((BYTE*)base + getplayerfrominstigator);

	return function(id);
}

bool is_friend(uintptr_t player)
{
	uintptr_t api_user = *(uintptr_t*)(player + 24);
	if (api_user != 0)
		return *(bool*)(api_user + 106); //bool <isFriend>k__BackingField;

	return false;
}