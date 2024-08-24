#pragma once

typedef struct {
    const char *text;
    int value;
} ENUM_STRING_MAP;

#define ENUM_STRING_MAP(type) g_EnumStr_##type

#ifdef ENUM_STR_IMPL
    #define DECLARE_ENUM_STRING_MAP(type, ...)                                 \
        const ENUM_STRING_MAP g_EnumStr_##type[] = { __VA_ARGS__ { NULL, -1 } };
#else
    #define DECLARE_ENUM_STRING_MAP(type, ...)                                 \
        extern const ENUM_STRING_MAP g_EnumStr_##type[];
#endif
