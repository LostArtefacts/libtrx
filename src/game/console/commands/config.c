#include "game/console/commands/config.h"

#include "config/config.h"
#include "config/config_map.h"
#include "game/game_string.h"
#include "memory.h"
#include "strings.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static const char *Console_Cmd_Config_Resolve(const char *option_name);
static bool Console_Cmd_Config_SameKey(const char *key1, const char *key2);

static const CONFIG_OPTION *Console_Cmd_Config_GetOptionFromKey(
    const char *key);

static bool Console_Cmd_Config_GetCurrentValue(
    const CONFIG_OPTION *option, char *target, size_t target_size);
static bool Console_Cmd_Config_SetCurrentValue(
    const CONFIG_OPTION *option, const char *new_value);

static COMMAND_RESULT Console_Cmd_Set(const char *args);

static const char *Console_Cmd_Config_Resolve(const char *const option_name)
{
    const char *dot = strrchr(option_name, '.');
    if (dot) {
        return dot + 1;
    }
    return option_name;
}

static bool Console_Cmd_Config_SameKey(const char *key1, const char *key2)
{
    key1 = Console_Cmd_Config_Resolve(key1);
    key2 = Console_Cmd_Config_Resolve(key2);
    const size_t len1 = strlen(key1);
    const size_t len2 = strlen(key2);
    if (len1 != len2) {
        return false;
    }
    for (uint32_t i = 0; i < len1; i++) {
        char c1 = key1[i];
        char c2 = key2[i];
        if (c1 == '_') {
            c1 = '-';
        }
        if (c2 == '_') {
            c2 = '-';
        }
        if (c1 != c2) {
            return false;
        }
    }
    return true;
}

static const CONFIG_OPTION *Console_Cmd_Config_GetOptionFromKey(
    const char *const key)
{
    for (const CONFIG_OPTION *option = Config_GetOptionMap();
         option->name != NULL; option++) {
        if (Console_Cmd_Config_SameKey(option->name, key)) {
            return option;
        }
    }

    return NULL;
}

static bool Console_Cmd_Config_GetCurrentValue(
    const CONFIG_OPTION *const option, char *target, const size_t target_size)
{
    if (option == NULL) {
        return false;
    }

    assert(option->target != NULL);
    switch (option->type) {
    case COT_BOOL:
        snprintf(
            target, target_size, "%s",
            *(bool *)option->target ? GS(MISC_ON) : GS(MISC_OFF));
        break;
    case COT_INT32:
        snprintf(target, target_size, "%d", *(int32_t *)option->target);
        break;
    case COT_FLOAT:
        snprintf(target, target_size, "%.2f", *(float *)option->target);
        break;
    case COT_DOUBLE:
        snprintf(target, target_size, "%.2f", *(double *)option->target);
        break;
    case COT_ENUM:
        for (const ENUM_STRING_MAP *enum_map = option->param;
             enum_map->text != NULL; enum_map++) {
            if (enum_map->value == *(int32_t *)option->target) {
                strncpy(target, enum_map->text, target_size);
            }
        }
        break;
    }
    return true;
}

static bool Console_Cmd_Config_SetCurrentValue(
    const CONFIG_OPTION *const option, const char *const new_value)
{
    if (option == NULL) {
        return CR_BAD_INVOCATION;
    }

    assert(option->target != NULL);
    switch (option->type) {
    case COT_BOOL:
        if (String_Match(new_value, "on|true|1")) {
            *(bool *)option->target = true;
            return true;
        } else if (String_Match(new_value, "off|false|0")) {
            *(bool *)option->target = false;
            return true;
        }
        break;

    case COT_INT32: {
        int32_t new_value_typed;
        if (sscanf(new_value, "%d", &new_value_typed) == 1) {
            *(int32_t *)option->target = new_value_typed;
            return true;
        }
        break;
    }

    case COT_FLOAT: {
        float new_value_typed;
        if (sscanf(new_value, "%f", &new_value_typed) == 1) {
            *(float *)option->target = new_value_typed;
            return true;
        }
        break;
    }

    case COT_DOUBLE: {
        double new_value_typed;
        if (sscanf(new_value, "%lf", &new_value_typed) == 1) {
            *(double *)option->target = new_value_typed;
            return true;
        }
        break;
    }

    case COT_ENUM:
        for (const ENUM_STRING_MAP *enum_map = option->param;
             enum_map->text != NULL; enum_map++) {
            if (String_Equivalent(enum_map->text, new_value)) {
                *(int32_t *)option->target = enum_map->value;
                return true;
            }
        }
        break;
    }

    return false;
}

static COMMAND_RESULT Console_Cmd_Config(const char *const args)
{
    COMMAND_RESULT result = CR_BAD_INVOCATION;

    char *key = Memory_DupStr(args);
    char *space = strchr(key, ' ');
    char *new_value = NULL;
    if (space != NULL) {
        new_value = space + 1;
        space[0] = '\0'; // NULL-terminate the key
    }

    const CONFIG_OPTION *const option =
        Console_Cmd_Config_GetOptionFromKey(key);
    if (option == NULL) {
        Console_Log(GS(OSD_CONFIG_OPTION_UNKNOWN_OPTION), key);
        result = CR_FAILURE;
        goto cleanup;
    }

    if (new_value != NULL) {
        if (Console_Cmd_Config_SetCurrentValue(option, new_value)) {
            Config_Sanitize();
            Config_Write();
            Config_ApplyChanges();

            char final_value[128];
            assert(
                Console_Cmd_Config_GetCurrentValue(option, final_value, 128));
            Console_Log(GS(OSD_CONFIG_OPTION_SET), key, final_value);
            result = CR_SUCCESS;
        }
    } else {
        char cur_value[128];
        if (Console_Cmd_Config_GetCurrentValue(option, cur_value, 128)) {
            Console_Log(GS(OSD_CONFIG_OPTION_GET), key, cur_value);
            result = CR_SUCCESS;
        }
    }

cleanup:
    Memory_FreePointer(&key);
    return result;
}

CONSOLE_COMMAND g_Console_Cmd_Config = {
    .prefix = "set",
    .proc = Console_Cmd_Config,
};
