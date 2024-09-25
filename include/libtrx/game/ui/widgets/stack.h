#pragma once

#include "./base.h"

#define UI_STACK_AUTO_SIZE (-1)

typedef enum {
    UI_STACK_LAYOUT_HORIZONTAL,
    UI_STACK_LAYOUT_VERTICAL,
    UI_STACK_LAYOUT_HORIZONTAL_INVERSE,
    UI_STACK_LAYOUT_VERTICAL_INVERSE,
} UI_STACK_LAYOUT;

UI_WIDGET *UI_Stack_Create(
    UI_STACK_LAYOUT layout, int32_t width, int32_t height);
void UI_Stack_AddChild(UI_WIDGET *self, UI_WIDGET *child);
void UI_Stack_SetSize(UI_WIDGET *widget, int32_t width, int32_t height);
void UI_Stack_DoLayout(UI_WIDGET *self);
