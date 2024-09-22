#pragma once

#include "./types.h"

#include <stdbool.h>

ITEM_INFO *Item_Get(int16_t num);
int32_t Item_GetTotalCount(void);
int32_t Item_GetDistance(const ITEM_INFO *item, const XYZ_32 *target);
void Item_TakeDamage(ITEM_INFO *item, int16_t damage, bool hit_status);
