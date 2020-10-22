#pragma once

#include <Windows.h>
#include <string>
#include <codecvt>

#define base (uintptr_t)GetModuleHandle("GameAssembly.dll")

void hook_function(PVOID* func, PVOID detour);

std::string utf16_to_utf8(std::u16string str);

bool is_local_player(uintptr_t p);