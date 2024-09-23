#pragma once

#include "../gl/buffer.h"
#include "../gl/program.h"
#include "../gl/sampler.h"
#include "../gl/texture.h"
#include "../gl/vertex_array.h"
#include "2d_surface.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    GFX_GL_VertexArray surface_format;
    GFX_GL_Buffer surface_buffer;
    GFX_GL_Texture surface_texture;
    GFX_GL_Sampler sampler;
    GFX_GL_Program program;
} GFX_2D_Renderer;

void GFX_2D_Renderer_Init(GFX_2D_Renderer *renderer);
void GFX_2D_Renderer_Close(GFX_2D_Renderer *renderer);

void GFX_2D_Renderer_Upload(
    GFX_2D_Renderer *renderer, GFX_2D_SurfaceDesc *desc, const uint8_t *data);
void GFX_2D_Renderer_Render(GFX_2D_Renderer *renderer);
