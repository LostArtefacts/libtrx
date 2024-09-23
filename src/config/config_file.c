#include "config/config_file.h"

#include "filesystem.h"
#include "log.h"
#include "memory.h"

#include <string.h>

static bool M_ReadFromJSON(
    const char *json, void (*load)(JSON_OBJECT *root_obj));
static char *M_WriteToJSON(void (*dump)(JSON_OBJECT *root_obj));
static const char *M_ResolveOptionName(const char *option_name);

static bool M_ReadFromJSON(
    const char *cfg_data, void (*load)(JSON_OBJECT *root_obj))
{
    bool result = false;

    JSON_PARSE_RESULT parse_result;
    JSON_VALUE *root = JSON_ParseEx(
        cfg_data, strlen(cfg_data), JSON_PARSE_FLAGS_ALLOW_JSON5, NULL, NULL,
        &parse_result);
    if (root != NULL) {
        result = true;
    } else {
        LOG_ERROR(
            "failed to parse config file: %s in line %d, char %d",
            JSON_GetErrorDescription(parse_result.error),
            parse_result.error_line_no, parse_result.error_row_no);
        // continue to supply the default values
    }

    JSON_OBJECT *root_obj = JSON_ValueAsObject(root);
    load(root_obj);

    if (root) {
        JSON_ValueFree(root);
    }

    return result;
}

static char *M_WriteToJSON(void (*dump)(JSON_OBJECT *root_obj))
{
    JSON_OBJECT *root_obj = JSON_ObjectNew();

    dump(root_obj);

    JSON_VALUE *root = JSON_ValueFromObject(root_obj);
    size_t size;
    char *data = JSON_WritePretty(root, "  ", "\n", &size);
    JSON_ValueFree(root);

    return data;
}

static const char *M_ResolveOptionName(const char *option_name)
{
    const char *dot = strrchr(option_name, '.');
    if (dot) {
        return dot + 1;
    }
    return option_name;
}

bool ConfigFile_Read(const char *path, void (*load)(JSON_OBJECT *root_obj))
{
    bool result = false;
    char *cfg_data = NULL;

    if (!File_Load(path, &cfg_data, NULL)) {
        LOG_WARNING("'%s' not loaded - default settings will apply", path);
        result = M_ReadFromJSON("{}", load);
    } else {
        result = M_ReadFromJSON(cfg_data, load);
    }

    Memory_FreePointer(&cfg_data);
    return result;
}

bool ConfigFile_Write(const char *path, void (*dump)(JSON_OBJECT *root_obj))
{
    LOG_INFO("Saving user settings");

    MYFILE *fp = File_Open(path, FILE_OPEN_WRITE);
    if (!fp) {
        return false;
    }

    char *data = M_WriteToJSON(dump);
    File_WriteData(fp, data, strlen(data));
    File_Close(fp);
    Memory_FreePointer(&data);

    return true;
}

void ConfigFile_LoadOptions(JSON_OBJECT *root_obj, const CONFIG_OPTION *options)
{
    const CONFIG_OPTION *opt = options;
    while (opt->target) {
        switch (opt->type) {
        case COT_BOOL:
            *(bool *)opt->target = JSON_ObjectGetBool(
                root_obj, M_ResolveOptionName(opt->name),
                *(bool *)opt->default_value);
            break;

        case COT_INT32:
            *(int32_t *)opt->target = JSON_ObjectGetInt(
                root_obj, M_ResolveOptionName(opt->name),
                *(int32_t *)opt->default_value);
            break;

        case COT_FLOAT:
            *(float *)opt->target = JSON_ObjectGetDouble(
                root_obj, M_ResolveOptionName(opt->name),
                *(float *)opt->default_value);
            break;

        case COT_DOUBLE:
            *(double *)opt->target = JSON_ObjectGetDouble(
                root_obj, M_ResolveOptionName(opt->name),
                *(double *)opt->default_value);
            break;

        case COT_ENUM:
            *(int *)opt->target = ConfigFile_ReadEnum(
                root_obj, M_ResolveOptionName(opt->name),
                *(int *)opt->default_value,
                (const ENUM_STRING_MAP *)opt->param);
        }
        opt++;
    }
}

void ConfigFile_DumpOptions(JSON_OBJECT *root_obj, const CONFIG_OPTION *options)
{
    const CONFIG_OPTION *opt = options;
    while (opt->target) {
        switch (opt->type) {
        case COT_BOOL:
            JSON_ObjectAppendBool(
                root_obj, M_ResolveOptionName(opt->name), *(bool *)opt->target);
            break;

        case COT_INT32:
            JSON_ObjectAppendInt(
                root_obj, M_ResolveOptionName(opt->name),
                *(int32_t *)opt->target);
            break;

        case COT_FLOAT:
            JSON_ObjectAppendDouble(
                root_obj, M_ResolveOptionName(opt->name),
                *(float *)opt->target);
            break;

        case COT_DOUBLE:
            JSON_ObjectAppendDouble(
                root_obj, M_ResolveOptionName(opt->name),
                *(double *)opt->target);
            break;

        case COT_ENUM:
            ConfigFile_WriteEnum(
                root_obj, M_ResolveOptionName(opt->name), *(int *)opt->target,
                (const ENUM_STRING_MAP *)opt->param);
            break;
        }
        opt++;
    }
}

int ConfigFile_ReadEnum(
    JSON_OBJECT *obj, const char *name, int default_value,
    const ENUM_STRING_MAP *enum_map)
{
    const char *value_str = JSON_ObjectGetString(obj, name, NULL);
    if (value_str) {
        while (enum_map->text) {
            if (!strcmp(value_str, enum_map->text)) {
                return enum_map->value;
            }
            enum_map++;
        }
    }
    return default_value;
}

void ConfigFile_WriteEnum(
    JSON_OBJECT *obj, const char *name, int value,
    const ENUM_STRING_MAP *enum_map)
{
    while (enum_map->text) {
        if (enum_map->value == value) {
            JSON_ObjectAppendString(obj, name, enum_map->text);
            break;
        }
        enum_map++;
    }
}
