#pragma once

#include <SDL2/SDL_stdinc.h>

typedef struct {
    Uint64 start;
} BENCHMARK;

BENCHMARK *Benchmark_Start(void);

#define Benchmark_End(...)                                                     \
    Benchmark_End_Impl(__FILE__, __LINE__, __func__, __VA_ARGS__)

void Benchmark_End_Impl(
    const char *file, int32_t line, const char *func, BENCHMARK *b,
    const char *message);
