#pragma once

#include "config_option.h"

bool Config_Read(void);
bool Config_Write(void);
void Config_Sanitize(void);
void Config_ApplyChanges(void);
const CONFIG_OPTION *Config_GetOptionMap(void);
