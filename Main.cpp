#include <Windows.h>
#include <thread>

#include "Helpers.h"
#include "IL2Cpp.h"
#include "Antis\AntiMasterDC.h"
#include "Antis\AntiOwnershipDesync.h"

void main()
{
	while (!GetModuleHandle("GameAssembly.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	initialize_il2cpp();
	initialize_anti_master_dc();
	initialize_anti_ownership_desync();

	//initialize confirmation
	Beep(500, 200);
}

bool __stdcall DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(main), NULL, NULL, nullptr);
	return true;
}