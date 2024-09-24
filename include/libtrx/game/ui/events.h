#pragma once

#include "./widgets/base.h"

typedef struct {
    const char *name;
    const UI_WIDGET *sender;
    void *data;
} UI_EVENT;

typedef void (*UI_EVENT_LISTENER)(const UI_EVENT *, void *user_data);

void UI_Events_Init(void);
void UI_Events_Shutdown(void);

int32_t UI_Events_Subscribe(
    const char *event_name, const UI_WIDGET *sender, UI_EVENT_LISTENER listener,
    void *user_data);

void UI_Events_Unsubscribe(int32_t listener_id);

void UI_Events_Fire(const UI_EVENT *event);
