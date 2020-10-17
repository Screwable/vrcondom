#pragma once

#include <Windows.h>
#include <stdint.h>

typedef int(__fastcall* triggerevent_fn)(uintptr_t, uintptr_t, uintptr_t, uint32_t, uint32_t);
extern triggerevent_fn original_triggerevent;
int trigger_event(uintptr_t _this, uintptr_t player, uintptr_t vrc_event, uint32_t broadcast_type, uint32_t instigator_id);

typedef int(__fastcall* oneventphoton_fn)(uintptr_t);
extern oneventphoton_fn original_onevent_photon;
int on_event_photon(uintptr_t event_data);

void initialize_events();