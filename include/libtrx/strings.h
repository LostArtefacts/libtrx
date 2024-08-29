#pragma once

#include <stdbool.h>
#include <stdint.h>

bool String_EndsWith(const char *str, const char *suffix);
bool String_Equivalent(const char *a, const char *b);

const char *String_CaseSubstring(const char *subject, const char *pattern);
int32_t String_Match(const char *subject, const char *pattern);

bool String_ParseBool(const char *value, bool *target);
bool String_ParseInteger(const char *value, int32_t *target);
bool String_ParseDecimal(const char *value, float *target);
