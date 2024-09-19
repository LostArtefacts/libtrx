#include "game/console/commands/give_item.h"

#include "game/backpack.h"
#include "game/game.h"
#include "game/game_string.h"
#include "game/lara/cheat.h"
#include "game/objects/common.h"
#include "game/objects/names.h"
#include "game/objects/vars.h"
#include "memory.h"
#include "strings.h"

#include <stdio.h>
#include <string.h>

static bool M_CanTargetObjectPickup(GAME_OBJECT_ID object_id);
static COMMAND_RESULT M_Entrypoint(const char *args);

static bool M_CanTargetObjectPickup(const GAME_OBJECT_ID object_id)
{
    return Object_IsObjectType(object_id, g_PickupObjects);
}

static COMMAND_RESULT M_Entrypoint(const char *args)
{
    if (!Game_IsPlayable()) {
        return CR_UNAVAILABLE;
    }

    if (String_Equivalent(args, "keys")) {
        return Lara_Cheat_GiveAllKeys() ? CR_SUCCESS : CR_FAILURE;
    }

    if (String_Equivalent(args, "guns")) {
        return Lara_Cheat_GiveAllGuns() ? CR_SUCCESS : CR_FAILURE;
    }

    if (String_Equivalent(args, "all")) {
        return Lara_Cheat_GiveAllItems() ? CR_SUCCESS : CR_FAILURE;
    }

    int32_t num = 1;
    if (sscanf(args, "%d ", &num) == 1) {
        args = strstr(args, " ");
        if (!args) {
            return CR_BAD_INVOCATION;
        }
        args++;
    }

    if (String_Equivalent(args, "")) {
        return CR_BAD_INVOCATION;
    }

    bool found = false;
    int32_t match_count = 0;
    GAME_OBJECT_ID *matching_objs =
        Object_IdsFromName(args, &match_count, M_CanTargetObjectPickup);
    for (int32_t i = 0; i < match_count; i++) {
        const GAME_OBJECT_ID object_id = matching_objs[i];
        if (Object_GetObject(object_id)->loaded) {
            Backpack_AddItemNTimes(object_id, num);
            Console_Log(GS(OSD_GIVE_ITEM), Object_GetName(object_id));
            found = true;
        }
    }
    Memory_FreePointer(&matching_objs);

    if (!found) {
        Console_Log(GS(OSD_INVALID_ITEM), args);
        return CR_FAILURE;
    }

    return CR_SUCCESS;
}

CONSOLE_COMMAND g_Console_Cmd_GiveItem = {
    .prefix = "give",
    .proc = M_Entrypoint,
};
