#include "game/game_string.h"

#include "memory.h"

#include <uthash.h>

typedef struct {
    char *key;
    char *value;
    UT_hash_handle hh;
} STRING_TABLE_ENTRY;

static STRING_TABLE_ENTRY *m_StringTable = NULL;

void GameString_Define(const char *key, const char *value)
{
    STRING_TABLE_ENTRY *entry;

    HASH_FIND_STR(m_StringTable, key, entry);
    if (entry == NULL) {
        entry = (STRING_TABLE_ENTRY *)Memory_Alloc(sizeof(STRING_TABLE_ENTRY));
        entry->key = Memory_DupStr(key);
        entry->value = Memory_DupStr(value);
        HASH_ADD_KEYPTR(
            hh, m_StringTable, entry->key, strlen(entry->key), entry);
    } else {
        Memory_Free(entry->value);
        entry->value = Memory_DupStr(value);
    }
}

bool GameString_IsKnown(const char *key)
{
    STRING_TABLE_ENTRY *entry;
    HASH_FIND_STR(m_StringTable, key, entry);
    return entry != NULL;
}

const char *GameString_Get(const char *key)
{
    STRING_TABLE_ENTRY *entry;
    HASH_FIND_STR(m_StringTable, key, entry);
    return entry ? entry->value : NULL;
}

void GameString_Clear(void)
{
    STRING_TABLE_ENTRY *entry, *tmp;

    HASH_ITER(hh, m_StringTable, entry, tmp)
    {
        HASH_DEL(m_StringTable, entry);
        Memory_Free(entry->key);
        Memory_Free(entry->value);
        Memory_Free(entry);
    }
}
