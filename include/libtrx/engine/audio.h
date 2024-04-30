#pragma once

#include <SDL2/SDL_audio.h>
#include <libavutil/samplefmt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define AUDIO_MAX_SAMPLES 1000
#define AUDIO_MAX_ACTIVE_SAMPLES 50
#define AUDIO_MAX_ACTIVE_STREAMS 10
#define AUDIO_NO_SOUND (-1)

bool Audio_Init(void);
bool Audio_Shutdown(void);

bool Audio_Stream_Pause(int32_t sound_id);
bool Audio_Stream_Unpause(int32_t sound_id);
int32_t Audio_Stream_CreateFromFile(const char *path);
bool Audio_Stream_Close(int32_t sound_id);
bool Audio_Stream_IsLooped(int32_t sound_id);
bool Audio_Stream_SetVolume(int32_t sound_id, float volume);
bool Audio_Stream_SetIsLooped(int32_t sound_id, bool is_looped);
bool Audio_Stream_SetFinishCallback(
    int32_t sound_id, void (*callback)(int32_t sound_id, void *user_data),
    void *user_data);
double Audio_Stream_GetTimestamp(int32_t sound_id);
double Audio_Stream_GetDuration(int32_t sound_id);
bool Audio_Stream_SeekTimestamp(int32_t sound_id, double timestamp);

bool Audio_Sample_ClearAll(void);
bool Audio_Sample_Load(size_t count, const char **contents, size_t *sizes);

int32_t Audio_Sample_Play(
    int32_t sample_id, int32_t volume, float pitch, int32_t pan,
    bool is_looped);
bool Audio_Sample_IsPlaying(int32_t sound_id);
bool Audio_Sample_Pause(int32_t sound_id);
bool Audio_Sample_PauseAll(void);
bool Audio_Sample_Unpause(int32_t sound_id);
bool Audio_Sample_UnpauseAll(void);
bool Audio_Sample_Close(int32_t sound_id);
bool Audio_Sample_CloseAll(void);
bool Audio_Sample_SetPan(int32_t sound_id, int32_t pan);
bool Audio_Sample_SetVolume(int32_t sound_id, int32_t volume);
