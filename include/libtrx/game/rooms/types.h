#pragma once

#include "../math.h"
#include "./enum.h"

#include <stdbool.h>

#if TR_VERSION == 1
typedef struct {
    TRIGGER_OBJECT type;
    void *parameter;
} TRIGGER_CMD;

typedef struct {
    int16_t camera_num;
    uint8_t timer;
    uint8_t glide;
    bool one_shot;
} TRIGGER_CAMERA_DATA;

typedef struct {
    TRIGGER_TYPE type;
    int8_t timer;
    int16_t mask;
    bool one_shot;
    int16_t item_index;
    int32_t command_count;
    TRIGGER_CMD *commands;
} TRIGGER;

typedef struct __PACKING {
    int16_t room_num;
    XYZ_16 normal;
    XYZ_16 vertex[4];
} DOOR_INFO;

#elif TR_VERSION == 2
typedef struct __PACKING {
    int16_t room;
    int16_t x;
    int16_t y;
    int16_t z;
    XYZ_16 vertex[4];
} DOOR_INFO;
#endif

typedef struct __PACKING {
    uint16_t count;
    DOOR_INFO door[];
} DOOR_INFOS;

#if TR_VERSION == 1
typedef struct __PACKING {
    uint16_t index;
    int16_t box;
    bool is_death_sector;
    TRIGGER *trigger;
    struct {
        uint8_t pit;
        uint8_t sky;
        int16_t wall;
    } portal_room;
    struct {
        int16_t height;
        int16_t tilt;
    } floor, ceiling;
} SECTOR_INFO;

typedef struct LIGHT_INFO {
    XYZ_32 pos;
    int16_t intensity;
    int32_t falloff;
} LIGHT_INFO;

typedef struct MESH_INFO {
    XYZ_32 pos;
    struct {
        int16_t y;
    } rot;
    uint16_t shade;
    uint16_t static_num;
} MESH_INFO;

typedef struct __PACKING {
    int16_t *data;
    DOOR_INFOS *doors;
    SECTOR_INFO *sectors;
    LIGHT_INFO *light;
    MESH_INFO *mesh;
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t min_floor;
    int32_t max_ceiling;
    int16_t z_size;
    int16_t x_size;
    int16_t ambient;
    int16_t num_lights;
    int16_t num_meshes;
    int16_t left;
    int16_t right;
    int16_t top;
    int16_t bottom;
    int16_t bound_active;
    int16_t item_num;
    int16_t fx_num;
    int16_t flipped_room;
    uint16_t flags;
} ROOM_INFO;

#elif TR_VERSION == 2
typedef struct __PACKING {
    uint16_t idx;
    int16_t box;
    uint8_t pit_room;
    int8_t floor;
    uint8_t sky_room;
    int8_t ceiling;
} SECTOR_INFO;

typedef struct __PACKING {
    int32_t x;
    int32_t y;
    int32_t z;
    int16_t intensity1;
    int16_t intensity2;
    int32_t falloff1;
    int32_t falloff2;
} LIGHT_INFO;

typedef struct __PACKING {
    int32_t x;
    int32_t y;
    int32_t z;
    int16_t y_rot;
    int16_t shade1;
    int16_t shade2;
    int16_t static_num;
} MESH_INFO;

typedef struct __PACKING {
    int16_t *data;
    DOOR_INFOS *doors;
    SECTOR_INFO *sector;
    LIGHT_INFO *light;
    MESH_INFO *mesh;
    XYZ_32 pos;
    int32_t min_floor;
    int32_t max_ceiling;
    int16_t z_size;
    int16_t x_size;
    int16_t ambient1;
    int16_t ambient2;
    int16_t light_mode;
    int16_t num_lights;
    int16_t num_meshes;
    int16_t bound_left;
    int16_t bound_right;
    int16_t bound_top;
    int16_t bound_bottom;
    uint16_t bound_active;
    int16_t test_left;
    int16_t test_right;
    int16_t test_top;
    int16_t test_bottom;
    int16_t item_num;
    int16_t fx_num;
    int16_t flipped_room;
    uint16_t flags;
} ROOM_INFO;
#endif
