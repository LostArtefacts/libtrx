#pragma once

#include "json.h"

enum bson_parse_error_e {
    bson_parse_error_none = 0,
    bson_parse_error_invalid_value,
    bson_parse_error_premature_end_of_buffer,
    bson_parse_error_unexpected_trailing_bytes,
    bson_parse_error_unknown,
};

struct bson_parse_result_s {
    enum bson_parse_error_e error;
    size_t error_offset;
};

// Parse a BSON file, returning a pointer to the root of the JSON structure.
// Returns NULL if an error occurred (malformed BSON input, or malloc failed).
struct json_value_s *bson_parse(const char *src, size_t src_size);

struct json_value_s *bson_parse_ex(
    const char *src, size_t src_size, struct bson_parse_result_s *result);

const char *bson_get_error_description(enum bson_parse_error_e error);

/* Write out a BSON binary string. Return 0 if an error occurred (malformed
 * JSON input, or malloc failed). The out_size parameter is optional. */
void *bson_write(const struct json_value_s *value, size_t *out_size);
