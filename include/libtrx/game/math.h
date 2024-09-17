#pragma once

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
    int32_t x;
    int32_t y;
    int32_t z;
} XYZ_32;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} XYZ_16;

#if TR_VERSION == 1
typedef enum {
    DIR_NORTH = 0,
    DIR_EAST = 1,
    DIR_SOUTH = 2,
    DIR_WEST = 3,
} DIRECTION;
#endif

#pragma pack(pop)
