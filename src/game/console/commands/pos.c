#include "game/console/commands/pos.h"

#include "game/console/common.h"
#include "game/const.h"
#include "game/game_string.h"
#include "game/lara/common.h"
#include "game/objects/common.h"

static COMMAND_RESULT M_Entrypoint(const char *args);

static COMMAND_RESULT M_Entrypoint(const char *const args)
{
    const OBJECT_INFO *const object = Object_GetObject(O_LARA);
    if (!object->loaded) {
        return CR_UNAVAILABLE;
    }

    const ITEM_INFO *const lara_item = Lara_GetItem();

    // clang-format off
    Console_Log(
        GS(OSD_POS_GET),
        lara_item->room_num,
        lara_item->pos.x / (float)WALL_L,
        lara_item->pos.y / (float)WALL_L,
        lara_item->pos.z / (float)WALL_L,
        lara_item->rot.x * 360.0f / (float)PHD_ONE,
        lara_item->rot.y * 360.0f / (float)PHD_ONE,
        lara_item->rot.z * 360.0f / (float)PHD_ONE);
    // clang-format on

    return CR_SUCCESS;
}

CONSOLE_COMMAND g_Console_Cmd_Pos = {
    .prefix = "pos",
    .proc = M_Entrypoint,
};
