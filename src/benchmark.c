#include "benchmark.h"

#include "log.h"
#include "memory.h"

#include <SDL2/SDL_timer.h>

BENCHMARK *Benchmark_Start(void)
{
    BENCHMARK *const b = Memory_Alloc(sizeof(BENCHMARK));
    b->start = SDL_GetPerformanceCounter();
    return b;
}

void Benchmark_End_Impl(
    const char *const file, const int32_t line, const char *const func,
    BENCHMARK *b, const char *const message)
{
    const double elapsed = (double)(SDL_GetPerformanceCounter() - b->start)
        * 1000.0 / (double)SDL_GetPerformanceFrequency();

    if (message == NULL) {
        Log_Message(file, line, func, "took %.02f ms", elapsed);
    } else {
        Log_Message(file, line, func, "%s: took %.02f ms", message, elapsed);
    }

    Memory_FreePointer(&b);
}
