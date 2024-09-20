#pragma once

#include "math.h"
#include "objects/ids.h"

#include <stdbool.h>
#include <stdint.h>

#define NO_ITEM (-1)

#pragma pack(push, 1)

#if TR_VERSION == 1
typedef enum {
    DS_CARRIED = 0,
    DS_FALLING = 1,
    DS_DROPPED = 2,
    DS_COLLECTED = 3,
} DROP_STATUS;

typedef struct CARRIED_ITEM {
    GAME_OBJECT_ID object_id;
    int16_t spawn_num;
    XYZ_32 pos;
    XYZ_16 rot;
    int16_t room_num;
    int16_t fall_speed;
    DROP_STATUS status;
    struct CARRIED_ITEM *next_item;
} CARRIED_ITEM;
#endif

typedef struct {
    int32_t floor;
    uint32_t touch_bits;
    uint32_t mesh_bits;
#if TR_VERSION == 1
    GAME_OBJECT_ID object_id;
#elif TR_VERSION == 2
    int16_t object_id;
#endif
    int16_t current_anim_state;
    int16_t goal_anim_state;
    int16_t required_anim_state;
    int16_t anim_num;
    int16_t frame_num;
    int16_t room_num;
    int16_t next_item;
    int16_t next_active;
    int16_t speed;
    int16_t fall_speed;
    int16_t hit_points;
    int16_t box_num;
    int16_t timer;
    uint16_t flags;

#if TR_VERSION == 1
    int16_t shade;
    void *data;
    void *priv;
    CARRIED_ITEM *carried_item;
#elif TR_VERSION == 2
    int16_t shade1;
    int16_t shade2;
    int16_t carried_item;
    void *data;
#endif

    XYZ_32 pos;
    XYZ_16 rot;

    uint16_t active : 1; // 0x0001
    uint16_t status : 2; // 0x0002…0x0004
    uint16_t gravity : 1; // 0x0008
    uint16_t hit_status : 1; // 0x0010
    uint16_t collidable : 1; // 0x0020
    uint16_t looked_at : 1; // 0x0040
#if TR_VERSION == 1
    uint16_t pad : 9; // 0x0080…0x8000
#elif TR_VERSION == 2
    uint16_t dynamic_light : 1; // 0x0080
    uint16_t killed : 1; // 0x0100
    uint16_t pad : 7; // 0x0200…0x8000
#endif

#if TR_VERSION == 1
    struct {
        struct {
            XYZ_32 pos;
            XYZ_16 rot;
        } result, prev;
    } interp;
#endif
} ITEM_INFO;

#pragma pack(pop)

ITEM_INFO *Item_Get(int16_t num);
int32_t Item_GetTotalCount(void);
int32_t Item_GetDistance(const ITEM_INFO *item, const XYZ_32 *target);
void Item_TakeDamage(ITEM_INFO *item, int16_t damage, bool hit_status);
