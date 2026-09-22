#pragma once

#include <stdlib.h>
#include <stdint.h>

#define LOGUE_CORE_MAP_WIDTH 80
#define LOGUE_CORE_MAP_HEIGHT 80

typedef struct {
    int seed;
} LogueCoreData;

typedef struct {
    int seed;
    uint8_t map[LOGUE_CORE_MAP_WIDTH][LOGUE_CORE_MAP_HEIGHT];
} LogueCoreMap;

class LogueCore {
public:
    LogueCore(int seed);

    void generate_map(LogueCoreMap* map);

private:
    LogueCoreData core_data;
    LogueCoreMap map;
};