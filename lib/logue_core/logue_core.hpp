#pragma once

#include <stdlib.h>
#include <stdint.h>

#define LOGUE_CORE_MAP_WIDTH 80
#define LOGUE_CORE_MAP_HEIGHT 80

typedef struct {
    int seed;
} logue_core_data_t;

typedef struct {
    int seed;
    uint8_t map[LOGUE_CORE_MAP_WIDTH][LOGUE_CORE_MAP_HEIGHT];
} logue_core_map_t;

class LogueCore {
public:
    LogueCore(int seed);

    void initialize(void);
    void generate_map(logue_core_map_t* map, int seed);

    int get_seed(void) { return core_data.seed; }

private:
    logue_core_data_t core_data;
    logue_core_map_t map;
};