#include "game/console/cmd/die.h"

#include "game/effects/exploding_death.h"
#include "game/items.h"
#include "game/lara/common.h"
#include "game/objects/common.h"
#include "game/objects/ids.h"
#include "game/sound.h"

static COMMAND_RESULT M_Entrypoint(const char *args);

static COMMAND_RESULT M_Entrypoint(const char *const args)
{
    if (!Object_GetObject(O_LARA)->loaded) {
        return CR_UNAVAILABLE;
    }

    LARA_INFO *const lara = Lara_GetLaraInfo();
    ITEM_INFO *const lara_item = Lara_GetItem();
    if (lara_item->hit_points <= 0) {
        return CR_UNAVAILABLE;
    }

    Sound_Effect(SFX_LARA_FALL, &lara_item->pos, SPM_NORMAL);
    Sound_Effect(SFX_EXPLOSION_CHEAT, &lara_item->pos, SPM_NORMAL);
    Effect_ExplodingDeath(lara->item_num, -1, 1);

    lara_item->hit_points = 0;
    lara_item->flags |= IF_INVISIBLE;
    return CR_SUCCESS;
}

CONSOLE_COMMAND g_Console_Cmd_Die = {
    .prefix = "abortion|natlastinks",
    .proc = M_Entrypoint,
};
