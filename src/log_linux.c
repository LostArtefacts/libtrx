#include "log.h"

#include <backtrace-supported.h>
#include <backtrace.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void Log_ErrorCallback(void *data, const char *msg, int errnum);
static int Log_FullTrace(
    void *data, uintptr_t pc, const char *filename, int lineno,
    const char *function);
static void Log_SignalHandler(int sig);

static void Log_ErrorCallback(void *data, const char *msg, int errnum)
{
    LOG_ERROR("%s", msg);
}

static int Log_FullTrace(
    void *data, uintptr_t pc, const char *filename, int lineno,
    const char *function)
{
    if (filename) {
        LOG_INFO(
            "0x%08X: %s (%s:%d)", pc, function ? function : "???",
            filename ? filename : "???", lineno);
    } else {
        LOG_INFO("0x%08X: %s", pc, function ? function : "???");
    }
    return 0;
}

static void Log_SignalHandler(int sig)
{
    LOG_ERROR("== CRASH REPORT ==");
    LOG_INFO("SIGNAL: %d", sig);
    LOG_INFO("STACK TRACE:");
    struct backtrace_state *state = backtrace_create_state(
        NULL, BACKTRACE_SUPPORTS_THREADS, Log_ErrorCallback, NULL);
    backtrace_full(state, 0, Log_FullTrace, Log_ErrorCallback, NULL);
    exit(EXIT_FAILURE);
}

void Log_Init_Extra(const char *path)
{
    signal(SIGSEGV, Log_SignalHandler);
    signal(SIGFPE, Log_SignalHandler);
}

void Log_Shutdown_Extra(void)
{
}
