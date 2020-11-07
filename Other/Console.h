#pragma once

#include <Windows.h>

typedef int(__fastcall* writeline_fn)(uintptr_t, const char*);
extern writeline_fn original_writeline;
int write_line(uintptr_t _this, const char* str);

void initialize_console_fix();