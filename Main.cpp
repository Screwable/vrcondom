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
#include "Other\Console.h"

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
	initialize_console_fix();

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
			printf((extensive_logging ? "Extensive logging was set to true\n" : "Extensive logging was set to false\n"));
		}
		if (arguments.at(0) == "antimasterdc")
		{
			anti_master_dc = !anti_master_dc;
			printf((anti_master_dc ? "Anti Master DC was set to true\n" : "Anti Master DC was set to false\n"));
		}
		if (arguments.at(0) == "antiownershipdesync")
		{
			anti_ownership_desync = !anti_ownership_desync;
			printf((anti_ownership_desync ? "Anti Ownership Desync was set to true\n" : "Anti Ownership Desync was set to false\n"));
		}
		if (arguments.at(0) == "antiworldtriggers")
		{
			anti_world_triggers = !anti_world_triggers;
			printf((anti_world_triggers ? "Anti World Triggers was set to true\n" : "Anti World Triggers was set to false\n"));
		}
		if (arguments.at(0) == "antiudon")
		{
			anti_udon_events = !anti_udon_events;
			printf((anti_udon_events ? "Anti Udon was set to true\n" : "Anti Udon was set to false\n"));
		}
		if (arguments.at(0) == "anticamerafreeze")
		{
			anti_camera_freeze = !anti_camera_freeze;
			printf((anti_camera_freeze ? "Anti Camera Freeze was set to true\n" : "Anti Camera Freeze was set to false\n"));
		}
		if (arguments.at(0) == "allowfriends")
		{
			allow_friends = !allow_friends;
			printf((allow_friends ? "Allow Friend Events was set to true\n" : "Allow Friend Events was set to false\n"));
		}
		if (arguments.at(0) == "help" || arguments.at(0) == "Help")
		{
			printf("\nlog :: Extensive logging\n\nantimasterdc :: Toggle Anti Master Disconnect\n\nantiownershipdesync :: Toggle Anti Desync\n\nantiworldtriggers :: Toggle Anti World Triggers\n\nantiudon :: Toggle Anti Udon Events\n\nanticamerafreeze :: Anti Camera Freeze Exploit\n\nallowfriends :: Allow Friends Events\n");
		}
		Sleep(1);
	}
}

bool __stdcall DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(main), NULL, NULL, nullptr); //reinterpret_cast looks nicer here
	return true;
}
