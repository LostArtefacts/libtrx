#pragma once

typedef struct {
    const char *text;
    int value;
} ENUM_STRING_MAP;

#define ENUM_SELF(type) { #type, type }
#define ENUM_STRING_MAP(type) g_EnumStr_##type

#ifdef ENUM_STR_IMPL
    #define DECLARE_ENUM_STRING_MAP(type, ...)                                 \
        const ENUM_STRING_MAP g_EnumStr_##type[] = { __VA_ARGS__ { NULL, -1 } };
#else
    #define DECLARE_ENUM_STRING_MAP(type, ...)                                 \
        extern const ENUM_STRING_MAP g_EnumStr_##type[];
#endif

#define ENUM_STRING_GET(type, key, default_value)                              \
    EnumStr_GetImpl(ENUM_STRING_MAP(type), (key), (default_value))

int EnumStr_GetImpl(
    const ENUM_STRING_MAP *enum_map, const char *key, int default_value);
