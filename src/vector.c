#include "vector.h"

#include "memory.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define VECTOR_DEFAULT_CAPACITY 4
#define VECTOR_GROWTH_RATE 2

static void Vector_EnsureCapacity(VECTOR *vector);

VECTOR *Vector_Create(const size_t item_size)
{
    return Vector_CreateAtCapacity(item_size, VECTOR_DEFAULT_CAPACITY);
}

VECTOR *Vector_CreateAtCapacity(const size_t item_size, const int32_t capacity)
{
    VECTOR *const vector = Memory_Alloc(sizeof(VECTOR));
    vector->count = 0;
    vector->capacity = capacity;
    vector->item_size = item_size;
    vector->items = Memory_Alloc(item_size * capacity);

    return vector;
}

void Vector_Free(VECTOR *vector)
{
    Memory_FreePointer(&vector->items);
    Memory_FreePointer(&vector);
}

static void Vector_EnsureCapacity(VECTOR *vector)
{
    if (vector->count + 1 <= vector->capacity) {
        return;
    }

    vector->capacity *= VECTOR_GROWTH_RATE;
    vector->items =
        Memory_Realloc(vector->items, vector->item_size * vector->capacity);
}

int32_t Vector_IndexOf(const VECTOR *vector, const void *item)
{
    for (int32_t i = 0; i < vector->count; i++) {
        if (vector->items[i] == item) {
            return i;
        }
    }
    return -1;
}

int32_t Vector_LastIndexOf(const VECTOR *vector, const void *item)
{
    for (int32_t i = vector->count - 1; i >= 0; i--) {
        if (vector->items[i] == item) {
            return i;
        }
    }
    return -1;
}

bool Vector_Contains(const VECTOR *vector, const void *item)
{
    return Vector_IndexOf(vector, item) != -1;
}

void Vector_Add(VECTOR *vector, void *item)
{
    Vector_EnsureCapacity(vector);
    vector->items[vector->count++] = item;
}

void Vector_Insert(VECTOR *vector, const int32_t index, void *item)
{
    assert(index >= 0 && index <= vector->count);

    Vector_EnsureCapacity(vector);

    for (int32_t i = vector->count - 1; i >= index; i--) {
        vector->items[i + 1] = vector->items[i];
    }

    vector->items[index] = item;
    vector->count++;
}

bool Vector_Remove(VECTOR *vector, const void *item)
{
    const int32_t index = Vector_IndexOf(vector, item);
    if (index == -1) {
        return false;
    }

    Vector_RemoveAt(vector, index);
    return true;
}

void Vector_RemoveAt(VECTOR *vector, const int32_t index)
{
    assert(index >= 0 && index < vector->count);

    vector->items[index] = NULL;
    for (int32_t i = index + 1; i < vector->count; i++) {
        vector->items[i - 1] = vector->items[i];
        vector->items[i] = NULL;
    }

    vector->count--;
}

void Vector_Reverse(VECTOR *vector)
{
    int32_t i = 0;
    int32_t j = vector->count - 1;
    for (; i < j; i++, j--) {
        void *temp = vector->items[i];
        vector->items[i] = vector->items[j];
        vector->items[j] = temp;
    }
}

void Vector_Clear(VECTOR *vector)
{
    for (int32_t i = 0; i < vector->count; i++) {
        vector->items[i] = NULL;
    }

    vector->count = 0;
    vector->capacity = VECTOR_DEFAULT_CAPACITY;
    vector->items =
        Memory_Realloc(vector->items, vector->item_size * vector->capacity);
}
