#include "audio.h"

#include "log.h"
#include "memory.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_stdinc.h>
#include <stdint.h>
#include <string.h>

SDL_AudioDeviceID g_AudioDeviceID = 0;
static int32_t m_RefCount = 0;
static size_t m_WorkingBufferSize = 0;
static float *m_WorkingBuffer = NULL;
static Uint8 m_WorkingSilence = 0;

static void Audio_MixerCallback(
    void *userdata, Uint8 *stream_data, int32_t len);

static void Audio_MixerCallback(void *userdata, Uint8 *stream_data, int32_t len)
{
    memset(m_WorkingBuffer, m_WorkingSilence, len);
    Audio_Stream_Mix(m_WorkingBuffer, len);
    Audio_Sample_Mix(m_WorkingBuffer, len);
    memcpy(stream_data, m_WorkingBuffer, len);
}

bool Audio_Init(void)
{
    m_RefCount++;
    if (g_AudioDeviceID) {
        // already initialized
        return true;
    }

    int32_t result = SDL_Init(SDL_INIT_AUDIO);
    if (result < 0) {
        LOG_ERROR("Error while calling SDL_Init: 0x%lx", result);
        return false;
    }

    Audio_Sample_Init();
    Audio_Stream_Init();

    SDL_AudioSpec desired;
    SDL_memset(&desired, 0, sizeof(desired));
    desired.freq = AUDIO_WORKING_RATE;
    desired.format = AUDIO_WORKING_FORMAT;
    desired.channels = AUDIO_WORKING_CHANNELS;
    desired.samples = AUDIO_SAMPLES;
    desired.callback = Audio_MixerCallback;
    desired.userdata = NULL;

    SDL_AudioSpec delivered;
    g_AudioDeviceID = SDL_OpenAudioDevice(NULL, 0, &desired, &delivered, 0);

    if (!g_AudioDeviceID) {
        LOG_ERROR("Failed to open audio device: %s", SDL_GetError());
        return false;
    }

    m_WorkingSilence = desired.silence;
    m_WorkingBufferSize = desired.samples * desired.channels
        * SDL_AUDIO_BITSIZE(desired.format) / 8;

    m_WorkingBuffer = Memory_Alloc(m_WorkingBufferSize);

    SDL_PauseAudioDevice(g_AudioDeviceID, 0);

    return true;
}

bool Audio_Shutdown(void)
{
    m_RefCount--;
    if (m_RefCount > 0) {
        return false;
    }

    Audio_Sample_Shutdown();
    Audio_Stream_Shutdown();

    if (g_AudioDeviceID) {
        SDL_PauseAudioDevice(g_AudioDeviceID, 1);
        SDL_CloseAudioDevice(g_AudioDeviceID);
        g_AudioDeviceID = 0;
    }

    Memory_FreePointer(&m_WorkingBuffer);
    return true;
}

int32_t Audio_GetAVChannelLayout(const int32_t channels)
{
    switch (channels) {
        // clang-format off
        case 1: return AV_CH_LAYOUT_MONO;
        case 2: return AV_CH_LAYOUT_STEREO;
        default: return AV_CH_LAYOUT_MONO;
        // clang-format on
    }
}

int32_t Audio_GetAVAudioFormat(const int32_t sample_fmt)
{
    switch (sample_fmt) {
        // clang-format off
        case AUDIO_U8: return AV_SAMPLE_FMT_U8;
        case AUDIO_S16: return AV_SAMPLE_FMT_S16;
        case AUDIO_S32: return AV_SAMPLE_FMT_S32;
        case AUDIO_F32: return AV_SAMPLE_FMT_FLT;
        default: return -1;
        // clang-format on
    }
}

int32_t Audio_GetSDLAudioFormat(const enum AVSampleFormat sample_fmt)
{
    // clang-format off
    switch (sample_fmt) {
        case AV_SAMPLE_FMT_U8: return AUDIO_U8;
        case AV_SAMPLE_FMT_S16: return AUDIO_S16;
        case AV_SAMPLE_FMT_S32: return AUDIO_S32;
        case AV_SAMPLE_FMT_FLT: return AUDIO_F32;
        default: return -1;
    }
    // clang-format on
}
