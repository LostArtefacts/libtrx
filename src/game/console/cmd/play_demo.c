#include "game/console/cmd/play_demo.h"

#include "game/gameflow/common.h"

static COMMAND_RESULT M_Entrypoint(const COMMAND_CONTEXT *ctx);

static COMMAND_RESULT M_Entrypoint(const COMMAND_CONTEXT *const ctx)
{
    Gameflow_OverrideCommand((GAMEFLOW_COMMAND) { .action = GF_START_DEMO });
    return CR_SUCCESS;
}

CONSOLE_COMMAND g_Console_Cmd_PlayDemo = {
    .prefix = "demo",
    .proc = M_Entrypoint,
};
