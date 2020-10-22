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

bool is_local_player(uintptr_t p)
{
	using fn = bool(*)(uintptr_t);
	fn function = (fn)((BYTE*)base + islocalplayer);

	return function(p);
}