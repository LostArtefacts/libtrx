#include "game/ui/widgets/stack.h"

#include "memory.h"
#include "utils.h"
#include "vector.h"

typedef struct {
    UI_WIDGET_VTABLE vtable;

    int32_t width;
    int32_t height;
    int32_t x;
    int32_t y;
    UI_STACK_LAYOUT layout;
    VECTOR *children;
} UI_STACK;

static int32_t M_GetHeight(const UI_STACK *self);
static int32_t M_GetWidth(const UI_STACK *self);
static void M_SetPosition(UI_STACK *self, int32_t x, int32_t y);
static void M_Control(UI_STACK *self);
static void M_Draw(UI_STACK *self);
static void M_Free(UI_STACK *self);

static int32_t M_GetWidth(const UI_STACK *const self)
{
    if (self->width != UI_STACK_AUTO_SIZE) {
        return self->width;
    }
    int32_t result = 0;
    for (int32_t i = 0; i < self->children->count; i++) {
        const UI_WIDGET *const child =
            *(UI_WIDGET **)Vector_Get(self->children, i);
        switch (self->layout) {
        case UI_STACK_LAYOUT_HORIZONTAL:
        case UI_STACK_LAYOUT_HORIZONTAL_INVERSE:
            result += child->get_width(child);
            break;
        case UI_STACK_LAYOUT_VERTICAL:
        case UI_STACK_LAYOUT_VERTICAL_INVERSE:
            result = MAX(result, child->get_width(child));
            break;
        }
    }
    return result;
}

static int32_t M_GetHeight(const UI_STACK *const self)
{
    if (self->height != UI_STACK_AUTO_SIZE) {
        return self->height;
    }
    int32_t result = 0;
    for (int32_t i = 0; i < self->children->count; i++) {
        const UI_WIDGET *const child =
            *(UI_WIDGET **)Vector_Get(self->children, i);
        switch (self->layout) {
        case UI_STACK_LAYOUT_HORIZONTAL:
        case UI_STACK_LAYOUT_HORIZONTAL_INVERSE:
            result = MAX(result, child->get_height(child));
            break;
        case UI_STACK_LAYOUT_VERTICAL:
        case UI_STACK_LAYOUT_VERTICAL_INVERSE:
            result += child->get_height(child);
            break;
        }
    }
    return result;
}

static void M_SetPosition(
    UI_STACK *const self, const int32_t x, const int32_t y)
{
    self->x = x;
    self->y = y;
    UI_Stack_DoLayout((UI_WIDGET *)self);
}

static void M_Control(UI_STACK *const self)
{
    for (int32_t i = 0; i < self->children->count; i++) {
        UI_WIDGET *const child = *(UI_WIDGET **)Vector_Get(self->children, i);
        if (child->control != NULL) {
            child->control(child);
        }
    }
}

static void M_Draw(UI_STACK *const self)
{
    for (int32_t i = 0; i < self->children->count; i++) {
        UI_WIDGET *const child = *(UI_WIDGET **)Vector_Get(self->children, i);
        if (child->draw != NULL) {
            child->draw(child);
        }
    }
}

static void M_Free(UI_STACK *const self)
{
    Vector_Free(self->children);
    Memory_Free(self);
}

void UI_Stack_AddChild(UI_WIDGET *const widget, UI_WIDGET *const child)
{
    UI_STACK *const self = (UI_STACK *)widget;
    Vector_Add(self->children, (void *)&child);
}

UI_WIDGET *UI_Stack_Create(
    const UI_STACK_LAYOUT layout, const int32_t width, const int32_t height)
{
    UI_STACK *const self = Memory_Alloc(sizeof(UI_STACK));
    self->vtable = (UI_WIDGET_VTABLE) {
        .control = (UI_WIDGET_CONTROL)M_Control,
        .draw = (UI_WIDGET_DRAW)M_Draw,
        .get_width = (UI_WIDGET_GET_WIDTH)M_GetWidth,
        .get_height = (UI_WIDGET_GET_HEIGHT)M_GetHeight,
        .set_position = (UI_WIDGET_SET_POSITION)M_SetPosition,
        .free = (UI_WIDGET_FREE)M_Free,
    };

    self->width = width;
    self->height = height;
    self->layout = layout;
    self->children = Vector_Create(sizeof(UI_WIDGET *));
    return (UI_WIDGET *)self;
}

void UI_Stack_SetSize(
    UI_WIDGET *const widget, const int32_t width, const int32_t height)
{
    UI_STACK *const self = (UI_STACK *)widget;
    self->width = width;
    self->height = height;
    UI_Stack_DoLayout(widget);
}

void UI_Stack_DoLayout(UI_WIDGET *const widget)
{
    UI_STACK *const self = (UI_STACK *)widget;
    const int32_t self_width = M_GetWidth(self);
    const int32_t self_height = M_GetHeight(self);
    const int32_t start_x = self->x;
    const int32_t start_y = self->y;
    int32_t x;
    int32_t y;

    switch (self->layout) {
    case UI_STACK_LAYOUT_HORIZONTAL:
    case UI_STACK_LAYOUT_VERTICAL:
        x = start_x;
        y = start_y;
        break;
    case UI_STACK_LAYOUT_HORIZONTAL_INVERSE:
    case UI_STACK_LAYOUT_VERTICAL_INVERSE:
        x = start_x + self_width;
        y = start_y + self_height;
        break;
    }

    for (int32_t i = 0; i < self->children->count; i++) {
        UI_WIDGET *const child = *(UI_WIDGET **)Vector_Get(self->children, i);
        const int32_t child_width = child->get_width(child);
        const int32_t child_height = child->get_height(child);

        // centre in the other axis
        switch (self->layout) {
        case UI_STACK_LAYOUT_HORIZONTAL:
        case UI_STACK_LAYOUT_HORIZONTAL_INVERSE:
            y = start_y + (self_height - child_height) / 2;
            break;
        case UI_STACK_LAYOUT_VERTICAL:
        case UI_STACK_LAYOUT_VERTICAL_INVERSE:
            x = start_x + (self_width - child_width) / 2;
            break;
        }

        child->set_position(child, x, y);

        switch (self->layout) {
        case UI_STACK_LAYOUT_HORIZONTAL_INVERSE:
            x -= child_width;
            break;
        case UI_STACK_LAYOUT_VERTICAL_INVERSE:
            y -= child_height;
            break;
        case UI_STACK_LAYOUT_HORIZONTAL:
            x += child_width;
            break;
        case UI_STACK_LAYOUT_VERTICAL:
            y += child_height;
            break;
        }
    }
}
