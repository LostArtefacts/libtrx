#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} IMAGE_PIXEL;

typedef struct {
    int32_t width;
    int32_t height;
    IMAGE_PIXEL *data;
} IMAGE;

IMAGE *Image_Create(int width, int height);
IMAGE *Image_CreateFromFile(const char *path);
void Image_Free(IMAGE *image);

bool Image_SaveToFile(const IMAGE *image, const char *path);

IMAGE *Image_ScaleFit(
    const IMAGE *source_image, size_t target_width, size_t target_height);

IMAGE *Image_ScaleCover(
    const IMAGE *source_image, size_t target_width, size_t target_height);

IMAGE *Image_ScaleStretch(
    const IMAGE *source_image, size_t target_width, size_t target_height);

IMAGE *Image_ScaleSmart(
    const IMAGE *source_image, size_t target_width, size_t target_height);
