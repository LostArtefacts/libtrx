#pragma once

typedef enum {
    NO_OBJECT = -1,
#define OBJ_ID_DEFINE(object_id, enum_value) object_id = enum_value,
#if TR_VERSION == 1
    #include "./ids_tr1.def"
#elif TR_VERSION == 2
    #include "./ids_tr2.def"
#endif
    O_NUMBER_OF,
} GAME_OBJECT_ID;
