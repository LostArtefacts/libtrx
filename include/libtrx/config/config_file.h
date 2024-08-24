#pragma once

#include "../enum_str.h"
#include "../json.h"
#include "config_option.h"

#include <stdbool.h>
#include <stdint.h>

bool ConfigFile_Read(
    const char *path, void (*load)(struct json_object_s *root_obj));
bool ConfigFile_Write(
    const char *path, void (*dump)(struct json_object_s *root_obj));

void ConfigFile_LoadOptions(
    struct json_object_s *root_obj, const CONFIG_OPTION *options);
void ConfigFile_DumpOptions(
    struct json_object_s *root_obj, const CONFIG_OPTION *options);

int ConfigFile_ReadEnum(
    struct json_object_s *obj, const char *name, int default_value,
    const ENUM_STRING_MAP *enum_map);
void ConfigFile_WriteEnum(
    struct json_object_s *obj, const char *name, int value,
    const ENUM_STRING_MAP *enum_map);
