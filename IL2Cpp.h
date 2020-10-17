#pragma once

#include <Windows.h>
#include <string>

typedef const char*(*il2cpp_string_chars_fn)(const char* str);
extern il2cpp_string_chars_fn il2cpp_string_chars;

std::string il2cpp_string_chars_to_string(const char* str);

void initialize_il2cpp();