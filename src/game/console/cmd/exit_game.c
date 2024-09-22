#include "game/console/cmd/exit_game.h"

#include "game/gameflow/common.h"

static COMMAND_RESULT M_Entrypoint(const COMMAND_CONTEXT *ctx);

static COMMAND_RESULT M_Entrypoint(const COMMAND_CONTEXT *const ctx)
{
    Gameflow_OverrideCommand((GAMEFLOW_COMMAND) { .action = GF_EXIT_GAME });
    return CR_SUCCESS;
}

CONSOLE_COMMAND g_Console_Cmd_ExitGame = {
    .prefix = "exit",
    .proc = M_Entrypoint,
};
