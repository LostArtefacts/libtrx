#pragma once

#include "../types.h"

#include <stdbool.h>

typedef enum {
    CR_SUCCESS,
    CR_FAILURE,
    CR_UNAVAILABLE,
    CR_BAD_INVOCATION,
} COMMAND_RESULT;

typedef struct __PACKING {
    const char *prefix;
    COMMAND_RESULT (*proc)(const char *args);
} CONSOLE_COMMAND;

void Console_Log(const char *fmt, ...);
COMMAND_RESULT Console_Eval(const char *cmdline);
