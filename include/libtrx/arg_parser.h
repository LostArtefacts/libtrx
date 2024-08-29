#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    ARG_PARSER_TYPE_BOOL,
    ARG_PARSER_TYPE_STRING,
    ARG_PARSER_TYPE_INTEGER,
    ARG_PARSER_TYPE_DECIMAL,
} ARG_PARSER_TYPE;

typedef struct {
    char *id;
    bool is_named;
    char *short_name; // for named arguments like -v
    char *long_name; // for named arguments like --verbose
    bool mandatory;
    ARG_PARSER_TYPE type;

    bool provided;
    struct {
        bool val_bool;
        char *val_string;
        int32_t val_integer;
        float val_decimal;
    } value;
} ARG_PARSER_ARGUMENT;

typedef struct {
    ARG_PARSER_ARGUMENT *args;
    int32_t argc;
    int32_t capacity;
} ARG_PARSER;

void ArgParser_Init(ARG_PARSER *const parser);
void ArgParser_Destroy(ARG_PARSER *const parser);

void ArgParser_AddArgument(
    ARG_PARSER *const parser, const ARG_PARSER_ARGUMENT *user_arg);

const ARG_PARSER_ARGUMENT *ArgParser_GetArgument(
    const ARG_PARSER *const parser, const char *id);

bool ArgParser_ParseCmdline(ARG_PARSER *parser, const char *cmdline);
bool ArgParser_ParseArgs(ARG_PARSER *parser, int32_t argc, const char **argv);
