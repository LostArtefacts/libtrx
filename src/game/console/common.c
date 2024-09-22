#include "game/console/common.h"

#include "log.h"
#include "memory.h"
#include "strings.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern int32_t Console_GetMaxLineLength(void);
extern void Console_LogImpl(const char *const text);

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
