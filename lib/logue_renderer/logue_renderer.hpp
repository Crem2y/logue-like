#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "logue_core.hpp"

class LogueRenderer {
public:
    LogueRenderer();

    void render_map(LogueCoreMap* map);
};