#include "game/ui/events.h"

#include "memory.h"
#include "vector.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    int32_t listener_id;
    const char *event_name;
    const UI_WIDGET *sender;
    UI_EVENT_LISTENER listener;
    void *user_data;
} M_LISTENER;

static VECTOR *m_Listeners = NULL;
static int32_t m_ListenerID = 0;

void UI_Events_Init(void)
{
    m_Listeners = Vector_Create(sizeof(M_LISTENER));
}

void UI_Events_Shutdown(void)
{
    Vector_Free(m_Listeners);
}

int32_t UI_Events_Subscribe(
    const char *const event_name, const UI_WIDGET *const sender,
    const UI_EVENT_LISTENER listener, void *const user_data)
{
    M_LISTENER entry = {
        .listener_id = m_ListenerID++,
        .event_name = event_name,
        .sender = sender,
        .listener = listener,
        .user_data = user_data,
    };
    Vector_Add(m_Listeners, &entry);
    return entry.listener_id;
}

void UI_Events_Unsubscribe(const int32_t listener_id)
{
    for (int32_t i = 0; i < m_Listeners->count; i++) {
        M_LISTENER entry = *(M_LISTENER *)Vector_Get(m_Listeners, i);
        if (entry.listener_id == listener_id) {
            Vector_RemoveAt(m_Listeners, i);
            return;
        }
    }
}

void UI_Events_Fire(const UI_EVENT *const event)
{
    for (int32_t i = 0; i < m_Listeners->count; i++) {
        M_LISTENER entry = *(M_LISTENER *)Vector_Get(m_Listeners, i);
        if (strcmp(entry.event_name, event->name) == 0
            && entry.sender == event->sender) {
            entry.listener(event, entry.user_data);
        }
    }
}
