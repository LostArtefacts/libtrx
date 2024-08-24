#include "config/config_file.h"

#include "filesystem.h"
#include "log.h"
#include "memory.h"

#include <string.h>

static bool Config_ReadFromJSON(
    const char *json, void (*load)(struct json_object_s *root_obj));
static char *Config_WriteToJSON(void (*dump)(struct json_object_s *root_obj));

static bool Config_ReadFromJSON(
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

static char *Config_WriteToJSON(void (*dump)(struct json_object_s *root_obj))
{
    struct json_object_s *root_obj = json_object_new();

    dump(root_obj);

    struct json_value_s *root = json_value_from_object(root_obj);
    size_t size;
    char *data = json_write_pretty(root, "  ", "\n", &size);
    json_value_free(root);

    return data;
}

bool ConfigFile_Read(
    const char *path, void (*load)(struct json_object_s *root_obj))
{
    bool result = false;
    char *cfg_data = NULL;

    if (!File_Load(path, &cfg_data, NULL)) {
        LOG_WARNING("'%s' not loaded - default settings will apply", path);
        result = Config_ReadFromJSON("{}", load);
    } else {
        result = Config_ReadFromJSON(cfg_data, load);
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

    char *data = Config_WriteToJSON(dump);
    File_WriteData(fp, data, strlen(data));
    File_Close(fp);
    Memory_FreePointer(&data);

    return true;
}
