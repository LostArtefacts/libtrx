#pragma once

#include "../collision.h"
#include "../items.h"
#include "../math.h"
#include "ids.h"

#include <stdint.h>

#if TR_VERSION == 1
typedef struct {
    XYZ_16 min;
    XYZ_16 max;
} BOUNDS_16;

typedef struct {
    struct {
        XYZ_16 min;
        XYZ_16 max;
    } shift, rot;
} OBJECT_BOUNDS;

typedef struct {
    BOUNDS_16 bounds;
    XYZ_16 offset;
    int16_t nmeshes;
    int32_t *mesh_rots;
} FRAME_INFO;

typedef struct {
    int16_t nmeshes;
    int16_t mesh_index;
    int32_t bone_index;
    FRAME_INFO *frame_base;
    void (*initialise)(int16_t item_num);
    void (*control)(int16_t item_num);
    int16_t (*floor_height_func)(
        const ITEM_INFO *item, int32_t x, int32_t y, int32_t z, int16_t height);
    int16_t (*ceiling_height_func)(
        const ITEM_INFO *item, int32_t x, int32_t y, int32_t z, int16_t height);
    void (*draw_routine)(ITEM_INFO *item);
    void (*collision)(int16_t item_num, ITEM_INFO *lara_item, COLL_INFO *coll);
    const OBJECT_BOUNDS *(*bounds)(void);
    int16_t anim_index;
    int16_t hit_points;
    int16_t pivot_length;
    int16_t radius;
    int16_t smartness;
    int16_t shadow_size;
    uint16_t loaded : 1;
    uint16_t intelligent : 1;
    uint16_t save_position : 1;
    uint16_t save_hitpoints : 1;
    uint16_t save_flags : 1;
    uint16_t save_anim : 1;
} OBJECT_INFO;

#elif TR_VERSION == 2
typedef struct __unaligned
{
    int16_t mesh_count;
    int16_t mesh_idx;
    int32_t bone_idx;
    int16_t *frame_base; // TODO: make me FRAME_INFO

    void (*initialise)(int16_t item_num);
    void (*control)(int16_t item_num);
    void (*floor)(
        const ITEM_INFO *item, int32_t x, int32_t y, int32_t z,
        int32_t *height);
    void (*ceiling)(
        const ITEM_INFO *item, int32_t x, int32_t y, int32_t z,
        int32_t *height);
    void (*draw_routine)(const ITEM_INFO *item);
    void (*collision)(int16_t item_num, ITEM_INFO *lara_item, COLL_INFO *coll);

    int16_t anim_idx;
    int16_t hit_points;
    int16_t pivot_length;
    int16_t radius;
    int16_t shadow_size;

    union {
        uint16_t flags;
        // clang-format off
        struct {
            uint16_t loaded:           1; // 0x01 1
            uint16_t intelligent:      1; // 0x02 2
            uint16_t save_position:    1; // 0x04 4
            uint16_t save_hitpoints:   1; // 0x08 8
            uint16_t save_flags:       1; // 0x10 16
            uint16_t save_anim:        1; // 0x20 32
            uint16_t semi_transparent: 1; // 0x40 64
            uint16_t water_creature:   1; // 0x80 128
            uint16_t pad:              8;
        };
        // clang-format on
    };
}
OBJECT_INFO;
#endif

OBJECT_INFO *Object_GetObject(GAME_OBJECT_ID object_id);

bool Object_IsObjectType(
    GAME_OBJECT_ID object_id, const GAME_OBJECT_ID *test_arr);
