#include "logue_core.hpp"

LogueCore::LogueCore(int seed) {
    core_data.seed = seed;
}

void LogueCore::initialize(void) {

}

void LogueCore::generate_map(logue_core_map_t* map, int seed) {
    map->seed = seed;

    srand(map->seed);
    rand();

    for (int x = 0; x < LOGUE_CORE_MAP_WIDTH; x++) {
        for (int y = 0; y < LOGUE_CORE_MAP_HEIGHT; y++) {
            map->map[x][y] = rand() % 2;
        }
    }
}