#include "game/console/cmd/fly.h"

#include "game/game.h"
#include "game/game_string.h"
#include "game/lara/cheat.h"

static COMMAND_RESULT M_Entrypoint(const char *const args);

static COMMAND_RESULT M_Entrypoint(const char *const args)
{
    if (!Game_IsPlayable()) {
        return CR_UNAVAILABLE;
    }
    Lara_Cheat_EnterFlyMode();
    return CR_SUCCESS;
}

CONSOLE_COMMAND g_Console_Cmd_Fly = {
    .prefix = "fly",
    .proc = M_Entrypoint,
};
