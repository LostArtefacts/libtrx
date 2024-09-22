#include "game/console/common.h"

#include "./extern.h"
#include "game/game_string.h"
#include "log.h"
#include "memory.h"
#include "strings.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void M_LogMultiline(const char *text);
static void M_Log(const char *text);

static void M_LogMultiline(const char *const text)
{
    assert(text != NULL);
    char *wrapped_text = String_WordWrap(text, Console_GetMaxLineLength());

    const char *start = wrapped_text;
    while (true) {
        const char *newline = strchr(start, '\n');
        if (newline == NULL) {
            break;
        }
        char temp[newline - start + 1];
        strncpy(temp, start, newline - start);
        temp[newline - start] = '\0';
        M_Log(temp);
        start = newline + 1;
    }
    if (*start != '\0') {
        M_Log(start);
    }
    Memory_FreePointer(&wrapped_text);
}

static void M_Log(const char *text)
{
    assert(text != NULL);
    Console_LogImpl(text);
}

void Console_Log(const char *fmt, ...)
{
    assert(fmt != NULL);

    va_list va;

    va_start(va, fmt);
    const size_t text_length = vsnprintf(NULL, 0, fmt, va);
    char *text = Memory_Alloc(text_length + 1);
    va_end(va);

    va_start(va, fmt);
    vsnprintf(text, text_length + 1, fmt, va);
    va_end(va);

    LOG_INFO("%s", text);
    M_LogMultiline(text);
    Memory_FreePointer(&text);
}

COMMAND_RESULT Console_Eval(const char *const cmdline)
{
    LOG_INFO("executing command: %s", cmdline);

    const char *args = NULL;
    const CONSOLE_COMMAND *matching_cmd = NULL;

    CONSOLE_COMMAND **cmd = Console_GetCommands();
    for (int32_t i = 0;; i++) {
        CONSOLE_COMMAND *cur_cmd = cmd[i];
        if (cur_cmd == NULL) {
            break;
        }

        char regex[strlen(cur_cmd->prefix) + 13];
        sprintf(regex, "^(%s)(\\s+.*)?$", cur_cmd->prefix);
        if (!String_Match(cmdline, regex)) {
            continue;
        }

        args = strstr(cmdline, " ");
        if (args != NULL) {
            args++;
        } else {
            args = "";
        }

        matching_cmd = cur_cmd;
        break;
    }

    if (matching_cmd == NULL) {
        Console_Log(GS(OSD_UNKNOWN_COMMAND), cmdline);
        return CR_BAD_INVOCATION;
    }

    assert(matching_cmd->proc != NULL);
    const COMMAND_RESULT result = matching_cmd->proc(args);

    switch (result) {
    case CR_BAD_INVOCATION:
        Console_Log(GS(OSD_COMMAND_BAD_INVOCATION), cmdline);
        break;

    case CR_UNAVAILABLE:
        Console_Log(GS(OSD_COMMAND_UNAVAILABLE));
        break;

    case CR_SUCCESS:
    case CR_FAILURE:
        // The commands themselves are responsible for handling logging in
        // these scenarios.
        break;
    }
    return result;
}
