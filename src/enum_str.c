#include "enum_str.h"

#include <string.h>

int EnumStr_GetImpl(
    const ENUM_STRING_MAP *enum_map, const char *const key,
    const int default_value)
{
    while (enum_map->text) {
        if (!strcmp(key, enum_map->text)) {
            return enum_map->value;
        }
        enum_map++;
    }
    return default_value;
}
