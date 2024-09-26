#include "config/common.h"

#include "config/file.h"
#include "game/ui/events.h"

bool Config_Read(void)
{
    const bool result = ConfigFile_Read(Config_GetPath(), &Config_LoadFromJSON);
    if (result) {
        Config_Sanitize();
        Config_ApplyChanges();
    }
    return result;
}

bool Config_Write(void)
{
    const EVENT event = {
        .name = "canvas_resize",
        .sender = NULL,
        .data = NULL,
    };
    UI_Events_Fire(&event);

    return ConfigFile_Write(Config_GetPath(), &Config_DumpToJSON);
}
