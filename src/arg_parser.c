#include "arg_parser.h"

#include "memory.h"
#include "string.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOKEN_BUFSIZE 64
#define TOKEN_DELIM " \t\r\n\a"

static void ArgParser_FillValue(ARG_PARSER_ARGUMENT *target, const char *value);
static bool ArgParser_ValueMatches(
    ARG_PARSER_ARGUMENT *target, const char *value);
static char **ArgParser_ShellSplit(const char *cmdline, int *argc);

static char **ArgParser_ShellSplit(const char *cmdline, int *const argc)
{
    int32_t bufsize = TOKEN_BUFSIZE;
    int32_t position = 0;
    char **tokens = Memory_Alloc(bufsize * sizeof(char *));
    char *token;
    const char *str_i;
    char *token_ptr;
    int32_t token_length;

    while (*cmdline) {
        while (isspace(*cmdline)) {
            cmdline++;
        }

        if (*cmdline == '\0') {
            break;
        }

        if (*cmdline == '"' || *cmdline == '\'') {
            char quote = *cmdline++;
            str_i = cmdline;
            token_ptr = token = Memory_Alloc(bufsize);
            while (*cmdline && *cmdline != quote) {
                if (*cmdline == '\\'
                    && (*(cmdline + 1) == quote || *(cmdline + 1) == '\\')) {
                    cmdline++; // Skip escape character
                    *token_ptr++ = *cmdline++;
                } else {
                    *token_ptr++ = *cmdline++;
                }

                if (token_ptr - token >= bufsize) {
                    bufsize += TOKEN_BUFSIZE;
                    token = Memory_Realloc(token, bufsize);
                    token_ptr = token + (token_ptr - token);
                }
            }
            *token_ptr = '\0';
            if (*cmdline) {
                cmdline++;
            }
        } else {
            str_i = cmdline;
            token_ptr = token = Memory_Alloc(bufsize);
            while (*cmdline && !isspace(*cmdline) && *cmdline != '"'
                   && *cmdline != '\'') {
                if (*cmdline == '\\'
                    && (*(cmdline + 1) == '"' || *(cmdline + 1) == '\''
                        || *(cmdline + 1) == '\\' || isspace(*(cmdline + 1)))) {
                    cmdline++; // Skip escape character
                    *token_ptr++ = *cmdline++;
                } else {
                    *token_ptr++ = *cmdline++;
                }
                if (token_ptr - token >= bufsize) {
                    bufsize += TOKEN_BUFSIZE;
                    token = Memory_Realloc(token, bufsize);
                    token_ptr = token + (token_ptr - token);
                }
            }
            *token_ptr = '\0';
        }

        tokens[position++] = token;
        if (position >= bufsize) {
            bufsize += TOKEN_BUFSIZE;
            tokens = Memory_Realloc(tokens, bufsize * sizeof(char *));
        }
    }
    tokens[position] = NULL;
    *argc = position;
    return tokens;
}

static void ArgParser_FillValue(
    ARG_PARSER_ARGUMENT *const target, const char *const value)
{
    switch (target->type) {
    case ARG_PARSER_TYPE_BOOL:
        String_ParseBool(value, &target->value.val_bool);
        break;
    case ARG_PARSER_TYPE_INTEGER:
        String_ParseInteger(value, &target->value.val_integer);
        break;
    case ARG_PARSER_TYPE_DECIMAL:
        String_ParseDecimal(value, &target->value.val_decimal);
        break;
    case ARG_PARSER_TYPE_STRING:
        if (target != NULL) {
            *target->value.val_string = Memory_DupStr(value);
        }
        break;
    }
}

static bool ArgParser_ValueMatches(
    ARG_PARSER_ARGUMENT *target, const char *value)
{
    // Checks whether a value can be coerced into a type.
    // This is to make sure optional positional arguments do not try to
    // populate numbers with strings.

    switch (target->type) {
    case ARG_PARSER_TYPE_BOOL:
        return String_ParseBool(value, NULL);
    case ARG_PARSER_TYPE_INTEGER:
        return String_ParseInteger(value, NULL);
    case ARG_PARSER_TYPE_DECIMAL:
        return String_ParseDecimal(value, NULL);
    case ARG_PARSER_TYPE_STRING:
        return true;
    }

    return false;
}

void ArgParser_Init(ARG_PARSER *const parser)
{
    parser->capacity = 10;
    parser->argc = 0;
    parser->args = Memory_Alloc(sizeof(ARG_PARSER_ARGUMENT) * parser->capacity);
}

void ArgParser_Destroy(ARG_PARSER *const parser)
{
    for (int32_t i = 0; i < parser->argc; i++) {
        ARG_PARSER_ARGUMENT *const arg = &parser->args[i];
        Memory_FreePointer(&arg->value.val_string);
        Memory_FreePointer(&arg->id);
        Memory_FreePointer(&arg->short_name);
        Memory_FreePointer(&arg->long_name);
    }
    Memory_FreePointer(&parser->args);
}

void ArgParser_AddArgument(
    ARG_PARSER *const parser, const ARG_PARSER_ARGUMENT *user_arg)
{
    if (parser->argc == parser->capacity) {
        parser->capacity *= 2;
        parser->args = Memory_Realloc(
            parser->args, sizeof(ARG_PARSER_ARGUMENT) * parser->capacity);
    }

    ARG_PARSER_ARGUMENT *const arg = &parser->args[parser->argc++];
    arg->id = Memory_DupStr(user_arg->id);
    arg->is_named = user_arg->is_named;
    arg->short_name =
        user_arg->short_name ? Memory_DupStr(user_arg->short_name) : NULL;
    arg->long_name =
        user_arg->long_name ? Memory_DupStr(user_arg->long_name) : NULL;
    arg->type = user_arg->type;
    arg->mandatory = user_arg->mandatory;

    arg->provided = false;
    arg->value.val_bool = false;
    arg->value.val_string = NULL;
    arg->value.val_integer = 0;
    arg->value.val_decimal = 0.0f;
}

const ARG_PARSER_ARGUMENT *ArgParser_GetArgument(
    const ARG_PARSER *const parser, const char *id)
{
    for (int32_t i = 0; i < parser->argc; i++) {
        ARG_PARSER_ARGUMENT *arg = &parser->args[i];
        if (strcmp(arg->id, id) == 0 && arg->provided) {
            return arg;
        }
    }
    return NULL;
}

bool ArgParser_ParseCmdline(ARG_PARSER *const parser, const char *const cmdline)
{
    int32_t argc;
    char **argv = ArgParser_ShellSplit(cmdline, &argc);
    const bool result = ArgParser_ParseArgs(parser, argc, (const char **)argv);
    for (int32_t i = 0; i < argc; i++) {
        Memory_FreePointer(&argv[i]);
    }
    Memory_FreePointer(&argv);
    return result;
}

bool ArgParser_ParseArgs(
    ARG_PARSER *const parser, const int32_t argc, const char **const argv)
{
    for (int32_t i = 0; i < argc; i++) {
        bool matched = false;

        for (int32_t j = 0; j < parser->argc; j++) {
            ARG_PARSER_ARGUMENT *arg = &parser->args[j];
            if (!arg->is_named) {
                continue;
            }

            if ((arg->short_name && strcmp(argv[i], arg->short_name) == 0)
                || (arg->long_name && strcmp(argv[i], arg->long_name) == 0)) {
                matched = true;
                if (arg->type == ARG_PARSER_TYPE_BOOL) {
                    arg->value.val_bool = true;
                } else {
                    if (++i >= argc) {
                        return false;
                    }
                    if (!ArgParser_ValueMatches(arg, argv[i])) {
                        return false;
                    }
                    ArgParser_FillValue(arg, argv[i]);
                }
                arg->provided = true;
                break;
            }
        }

        if (!matched) {
            for (int32_t j = 0; j < parser->argc; j++) {
                ARG_PARSER_ARGUMENT *arg = &parser->args[j];
                if (!arg->is_named && !arg->provided
                    && ArgParser_ValueMatches(arg, argv[i])) {
                    matched = true;
                    ArgParser_FillValue(arg, argv[i]);
                    arg->provided = true;
                    break;
                }
            }

            if (!matched) {
                return false;
            }
        }
    }

    for (int32_t i = 0; i < parser->argc; i++) {
        if (parser->args[i].mandatory && !parser->args[i].provided) {
            return false;
        }
    }

    return true;
}
