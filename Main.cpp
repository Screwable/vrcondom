#include <Windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>

#include "Helpers.h"
#include "IL2Cpp.h"
#include "Antis\Events.h"
#include "Settings.h"

//dont know where this function is from, possibly stackoverflow(?), either way it works
std::vector<std::string> split(std::string str)
{
	std::stringstream stream(str);
	std::istream_iterator<std::string> start(stream);
	std::istream_iterator<std::string> end;
	std::vector<std::string> vec(start, end);
	return vec;
}

void main()
{
	while (!GetModuleHandle("GameAssembly.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

	AllocConsole();
	SetConsoleTitle("VRChat");
	FILE* in = stdin, * out = stdout;
	freopen_s(&in, "conin$", "r", stdin);
	freopen_s(&out, "conout$", "w", stdout);

	initialize_il2cpp();
	initialize_events();

	//initialize confirmation
	Beep(500, 200);

	while (true)
	{
		printf("\n>");

		std::string command;
		std::cin >> command;

		std::vector<std::string> arguments = split(command);

		if (arguments.at(0) == "log")
		{
			extensive_logging = !extensive_logging;
			printf("Extensive logging was toggled\n");
		}
		if (arguments.at(0) == "antimasterdc")
		{
			anti_master_dc = !anti_master_dc;
			printf("Anti Master DC was toggled\n");
		}
		if (arguments.at(0) == "antiownershipdesync")
		{
			anti_ownership_desync = !anti_ownership_desync;
			printf("Anti Ownership Desync was toggled\n");
		}
		if (arguments.at(0) == "antiworldtriggers")
		{
			anti_world_triggers = !anti_world_triggers;
			printf("Anti World Triggers was toggled\n");
		}
		if (arguments.at(0) == "antiudon")
		{
			anti_udon_events = !anti_udon_events;
			printf("Anti Udon was toggled\n");
		}

		Sleep(1);
	}
}

bool __stdcall DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(main), NULL, NULL, nullptr);
	return true;
}