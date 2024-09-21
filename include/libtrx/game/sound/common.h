#pragma once

#include "../math.h"
#include "ids.h"

// clang-format off
typedef enum SOUND_PLAY_MODE {
    SPM_NORMAL     = 0,
    SPM_UNDERWATER = 1,
    SPM_ALWAYS     = 2,
} SOUND_PLAY_MODE;
// clang-format on

bool Sound_Effect(int32_t sfx_num, const XYZ_32 *pos, uint32_t flags);
