#include "config/config_file.h"

#include "filesystem.h"
#include "log.h"
#include "memory.h"

#include <string.h>

static bool M_ReadFromJSON(
    const char *json, void (*load)(struct json_object_s *root_obj));
static char *M_WriteToJSON(void (*dump)(struct json_object_s *root_obj));
static const char *M_ResolveOptionName(const char *option_name);

static bool M_ReadFromJSON(
    const char *cfg_data, void (*load)(struct json_object_s *root_obj))
{
    bool result = false;
    struct json_value_s *root;
    struct json_parse_result_s parse_result;

    root = json_parse_ex(
        cfg_data, strlen(cfg_data), json_parse_flags_allow_json5, NULL, NULL,
        &parse_result);
    if (root) {
        result = true;
    } else {
        LOG_ERROR(
            "failed to parse config file: %s in line %d, char %d",
            json_get_error_description(parse_result.error),
            parse_result.error_line_no, parse_result.error_row_no);
        // continue to supply the default values
    }

    struct json_object_s *root_obj = json_value_as_object(root);
    load(root_obj);

    if (root) {
        json_value_free(root);
    }

    return result;
}

static char *M_WriteToJSON(void (*dump)(struct json_object_s *root_obj))
{
    struct json_object_s *root_obj = json_object_new();

    dump(root_obj);

    struct json_value_s *root = json_value_from_object(root_obj);
    size_t size;
    char *data = json_write_pretty(root, "  ", "\n", &size);
    json_value_free(root);

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

bool ConfigFile_Read(
    const char *path, void (*load)(struct json_object_s *root_obj))
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

bool ConfigFile_Write(
    const char *path, void (*dump)(struct json_object_s *root_obj))
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

void ConfigFile_LoadOptions(
    struct json_object_s *root_obj, const CONFIG_OPTION *options)
{
    const CONFIG_OPTION *opt = options;
    while (opt->target) {
        switch (opt->type) {
        case COT_BOOL:
            *(bool *)opt->target = json_object_get_bool(
                root_obj, M_ResolveOptionName(opt->name),
                *(bool *)opt->default_value);
            break;

        case COT_INT32:
            *(int32_t *)opt->target = json_object_get_int(
                root_obj, M_ResolveOptionName(opt->name),
                *(int32_t *)opt->default_value);
            break;

        case COT_FLOAT:
            *(float *)opt->target = json_object_get_double(
                root_obj, M_ResolveOptionName(opt->name),
                *(float *)opt->default_value);
            break;

        case COT_DOUBLE:
            *(double *)opt->target = json_object_get_double(
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

void ConfigFile_DumpOptions(
    struct json_object_s *root_obj, const CONFIG_OPTION *options)
{
    const CONFIG_OPTION *opt = options;
    while (opt->target) {
        switch (opt->type) {
        case COT_BOOL:
            json_object_append_bool(
                root_obj, M_ResolveOptionName(opt->name), *(bool *)opt->target);
            break;

        case COT_INT32:
            json_object_append_int(
                root_obj, M_ResolveOptionName(opt->name),
                *(int32_t *)opt->target);
            break;

        case COT_FLOAT:
            json_object_append_double(
                root_obj, M_ResolveOptionName(opt->name),
                *(float *)opt->target);
            break;

        case COT_DOUBLE:
            json_object_append_double(
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
    struct json_object_s *obj, const char *name, int default_value,
    const ENUM_STRING_MAP *enum_map)
{
    const char *value_str = json_object_get_string(obj, name, NULL);
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
    struct json_object_s *obj, const char *name, int value,
    const ENUM_STRING_MAP *enum_map)
{
    while (enum_map->text) {
        if (enum_map->value == value) {
            json_object_append_string(obj, name, enum_map->text);
            break;
        }
        enum_map++;
    }
}
