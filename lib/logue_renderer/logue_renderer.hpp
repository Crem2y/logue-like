#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "logue_core.hpp"

typedef struct {
    int temp;
} logue_renderer_config_t;

class LogueRenderer {
public:
    LogueRenderer(void);

    void render_map(logue_map_t* map, logue_renderer_config_t* config);
};