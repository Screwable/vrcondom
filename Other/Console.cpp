#include "Console.h"
#include "..\Helpers.h"
#include "..\IL2Cpp.h"
#include "..\Addresses.h"

writeline_fn original_writeline = NULL;
int write_line(uintptr_t _this, const char* str)
{
	using stacktrace_fn = const char*(*)();
	stacktrace_fn get_stacktrace = (stacktrace_fn)((BYTE*)base + stacktrace);

	if (get_stacktrace() != "" && il2cpp_string_chars_to_string(get_stacktrace()).find("Transmtn") != std::string::npos)
		return NULL;

	return original_writeline(_this, str);
}

void initialize_console_fix()
{
	original_writeline = writeline_fn(base + writeline);
	hook_function((PVOID*)(&original_writeline), write_line);
}
