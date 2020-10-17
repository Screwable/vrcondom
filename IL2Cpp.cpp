#include "IL2Cpp.h"
#include "Helpers.h"

il2cpp_string_chars_fn il2cpp_string_chars = NULL;

std::string il2cpp_string_chars_to_string(const char* str)
{
	std::string s = utf16_to_utf8((const char16_t*)il2cpp_string_chars(str));
	if (s.empty() || s.size() == 0)
		return "empty string";

	if (s != "")
		return s;
	else
		return "empty string";
}

void initialize_il2cpp()
{
	//so we can use il2cpp_string_chars_to_string :)
	il2cpp_string_chars = (il2cpp_string_chars_fn)GetProcAddress(GetModuleHandle("GameAssembly.dll"), "il2cpp_string_chars");
}