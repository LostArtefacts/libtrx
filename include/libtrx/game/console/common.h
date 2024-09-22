#pragma once

#include "../types.h"

#include <stdbool.h>

typedef enum {
    CR_SUCCESS,
    CR_FAILURE,
    CR_UNAVAILABLE,
    CR_BAD_INVOCATION,
} COMMAND_RESULT;

typedef struct {
    const struct __PACKING CONSOLE_COMMAND *cmd;
    const char *prefix;
    const char *args;
} COMMAND_CONTEXT;

typedef struct __PACKING CONSOLE_COMMAND {
    const char *prefix;
    COMMAND_RESULT (*proc)(const COMMAND_CONTEXT *ctx);
} CONSOLE_COMMAND;

void Console_Log(const char *fmt, ...);
COMMAND_RESULT Console_Eval(const char *cmdline);
