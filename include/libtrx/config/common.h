#pragma once

#include "../json.h"
#include "./option.h"

#include <stdbool.h>

bool Config_Read(void);
bool Config_Write(void);

extern const char *Config_GetPath(void);
extern void Config_Sanitize(void);
extern void Config_ApplyChanges(void);
extern const CONFIG_OPTION *Config_GetOptionMap(void);
extern void Config_LoadFromJSON(JSON_OBJECT *root_obj);
extern void Config_DumpToJSON(JSON_OBJECT *root_obj);
