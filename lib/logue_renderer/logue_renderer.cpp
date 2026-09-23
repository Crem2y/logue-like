#include "logue_renderer.hpp"

LogueRenderer::LogueRenderer() {

}

void LogueRenderer::render_map(logue_map_t* map, logue_renderer_config_t* config) {
    printf("map seed = %d\n", map->seed);

    for (uint16_t y = 0; y < LOGUE_MAP_HEIGHT; y++) {
        for (uint16_t x = 0; x < LOGUE_MAP_WIDTH; x++) {
            switch(map->map[x][y]) {
            case MAP_FLOOR:
                printf(".");
                break;
            case MAP_WALL:
                printf("#");
                break;
            case MAP_STAIR:
                printf("/");
                break;
            case MAP_PLAYER:
                printf("@");
                break;
            case MAP_ENEMY:
                printf("!");
                break;
            case MAP_ITEM:
                printf("*");
                break;
            case MAP_BLANK:
            default:
                printf(" ");
                break;
            }
        }
        printf("\n");
    }
}