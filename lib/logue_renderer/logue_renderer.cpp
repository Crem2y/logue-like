#include "logue_renderer.hpp"

LogueRenderer::LogueRenderer() {

}

void LogueRenderer::render_map(LogueCoreMap* map) {
    printf("seed: %d\n", map->seed);
    for (int y = 0; y < LOGUE_CORE_MAP_HEIGHT; y++) {
        for (int x = 0; x < LOGUE_CORE_MAP_WIDTH; x++) {
            printf("%c ", map->map[x][y] ? '*' : ' ');
        }
        printf("\n");
    }
}