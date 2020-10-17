#pragma once

#include <Windows.h>
#include <stdint.h>

typedef int(__fastcall* triggerevent_fn)(uintptr_t, uintptr_t, uintptr_t, uint32_t, uint32_t);
extern triggerevent_fn original_triggerevent;
int trigger_event(uintptr_t _this, uintptr_t player, uintptr_t vrc_event, uint32_t broadcast_type, uint32_t instigator_id);

void initialize_anti_master_dc();