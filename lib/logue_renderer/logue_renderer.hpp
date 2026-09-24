#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "logue_core.hpp"

enum logue_render_visibility {
    RENDER_NONE       = 0x00,
    RENDER_DISCOVERED = VIS_DISCOVERED,
    RENDER_VISIBLE    = VIS_VISIBLE,
    RENDER_SEARCHED   = VIS_SEARCHED,
    RENDER_ALWAYS     = 0x80,
};

typedef struct {
    uint8_t map;
    uint8_t stair;
    uint8_t player;
    uint8_t enemy;
    uint8_t item;
} logue_renderer_config_t;

class LogueRenderer {
public:
    LogueRenderer(void);

    void render_map(logue_map_t* map, logue_renderer_config_t* config);
    bool is_renderable(uint8_t visibility, uint8_t config);
};