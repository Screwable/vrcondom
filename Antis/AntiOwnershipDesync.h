#pragma once

#include <Windows.h>

typedef int(__fastcall* oneventphoton_fn)(uintptr_t);
extern oneventphoton_fn original_onevent_photon;
int on_event_photon(uintptr_t event_data);

void initialize_anti_ownership_desync();