#pragma once

#include "../json.h"

#include <stdbool.h>
#include <stdint.h>

bool ConfigFile_Read(
    const char *path, void (*load)(struct json_object_s *root_obj));
bool ConfigFile_Write(
    const char *path, void (*dump)(struct json_object_s *root_obj));
