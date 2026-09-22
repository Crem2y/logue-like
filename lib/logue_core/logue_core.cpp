#include "logue_core.hpp"

LogueCore::LogueCore(int seed) {
    core_data.seed = seed;
}

void LogueCore::generate_map(LogueCoreMap* map) {
    map->seed = core_data.seed;

    srand(core_data.seed);
    rand();

    for (int x = 0; x < LOGUE_CORE_MAP_WIDTH; x++) {
        for (int y = 0; y < LOGUE_CORE_MAP_HEIGHT; y++) {
            map->map[x][y] = rand() % 2;
        }
    }
}