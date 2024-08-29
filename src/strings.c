#include "strings.h"

#include "log.h"

#include <ctype.h>
#include <pcre2.h>
#include <string.h>

bool String_EndsWith(const char *str, const char *suffix)
{
    int str_len = strlen(str);
    int suffix_len = strlen(suffix);

    if (suffix_len > str_len) {
        return 0;
    }

    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

bool String_Equivalent(const char *a, const char *b)
{
    if (a == NULL || b == NULL) {
        return false;
    }

    size_t a_size = strlen(a);
    size_t b_size = strlen(b);
    if (a_size != b_size) {
        return false;
    }

    for (size_t i = 0; i < a_size; i++) {
        if (tolower(a[i]) != tolower(b[i])) {
            return false;
        }
    }
    return true;
}

const char *String_CaseSubstring(const char *subject, const char *pattern)
{
    if (subject == NULL || pattern == NULL) {
        return NULL;
    }

    size_t str_size = strlen(subject);
    size_t substr_size = strlen(pattern);
    if (substr_size > str_size) {
        return NULL;
    }
    if (substr_size == 0) {
        return subject;
    }

    for (size_t i = 0; i < str_size + 1 - substr_size; i++) {
        bool equivalent = true;
        for (size_t j = 0; j < substr_size; j++) {
            if (tolower(subject[i + j]) != tolower(pattern[j])) {
                equivalent = false;
                break;
            }
        }
        if (equivalent) {
            return subject + i;
        }
    }
    return NULL;
}

int32_t String_Match(const char *const subject, const char *const pattern)
{
    if (subject == NULL || pattern == NULL) {
        return 0;
    }

    const unsigned char *const usubject = (const unsigned char *)subject;
    const unsigned char *const upattern = (const unsigned char *)pattern;
    size_t pattern_size = strlen(pattern);
    size_t subject_size = strlen(subject);
    uint32_t options = PCRE2_CASELESS;

    pcre2_match_data *match_data;
    uint32_t ovecsize = 128;

    int errcode;
    PCRE2_SIZE erroffset;
    pcre2_code *re = pcre2_compile(
        upattern, pattern_size, options, &errcode, &erroffset, NULL);
    if (re == NULL) {
        PCRE2_UCHAR8 buffer[128];
        pcre2_get_error_message(errcode, buffer, 120);
        LOG_ERROR("%d\t%s", errcode, buffer);
        return false;
    }

    match_data = pcre2_match_data_create(ovecsize, NULL);
    const int rc =
        pcre2_match(re, usubject, subject_size, 0, options, match_data, NULL);
    PCRE2_SIZE match_length = 0;
    if (rc > 0) {
        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
        match_length = ovector[1] - ovector[0];
    }
    pcre2_match_data_free(match_data);
    pcre2_code_free(re);

    return match_length;
}

bool String_ParseBool(const char *const value, bool *const target)
{
    if (String_Match(value, "0|false|off")) {
        if (target != NULL) {
            *target = false;
        }
        return true;
    }

    if (String_Match(value, "1|true|on")) {
        if (target != NULL) {
            *target = true;
        }
        return true;
    }

    return false;
}

bool String_ParseInteger(const char *const value, int32_t *const target)
{
    for (size_t i = 0; i < strlen(value); i++) {
        if (i == 0 && value[i] == '-') {
            continue;
        }
        if (!isdigit(value[i])) {
            return false;
        }
    }
    if (target != NULL) {
        *target = atoi(value);
    }
    return true;
}

bool String_ParseDecimal(const char *const value, float *const target)
{
    bool has_dot = false;
    for (size_t i = 0; i < strlen(value); i++) {
        if (i == 0 && value[i] == '-') {
            continue;
        }
        if (!isdigit(value[i])) {
            if (value[i] == '.') {
                if (has_dot) {
                    return false;
                }
                has_dot = true;
            } else {
                return false;
            }
        }
    }
    if (target != NULL) {
        *target = atof(value);
    }
    return true;
}
