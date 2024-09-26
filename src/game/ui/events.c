#include "game/ui/events.h"

#include <stddef.h>

static EVENT_MANAGER *m_EventManager = NULL;

void UI_Events_Init(void)
{
    m_EventManager = EventManager_Create();
}

void UI_Events_Shutdown(void)
{
    EventManager_Free(m_EventManager);
    m_EventManager = NULL;
}

int32_t UI_Events_Subscribe(
    const char *const event_name, const UI_WIDGET *const sender,
    const EVENT_LISTENER listener, void *const user_data)
{
    return EventManager_Subscribe(
        m_EventManager, event_name, sender, listener, user_data);
}

void UI_Events_Unsubscribe(const int32_t listener_id)
{
    EventManager_Unsubscribe(m_EventManager, listener_id);
}

void UI_Events_Fire(const EVENT *const event)
{
    EventManager_Fire(m_EventManager, event);
}
