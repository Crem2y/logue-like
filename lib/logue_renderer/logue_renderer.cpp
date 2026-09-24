#include "logue_renderer.hpp"

LogueRenderer::LogueRenderer() {

}

void LogueRenderer::render_map(logue_map_t* map, logue_renderer_config_t* config) {
    if(map == NULL || config == NULL) return;

    printf("map seed = %d, map difficulty = %d\n", map->seed, map->difficulty); //test

    uint8_t rendering_map[LOGUE_MAP_HEIGHT][LOGUE_MAP_WIDTH];

    // copy terrain
    for (uint16_t y = 0; y < LOGUE_MAP_HEIGHT; y++) {
        for (uint16_t x = 0; x < LOGUE_MAP_WIDTH; x++) {

            if (!is_renderable(map->visibility[y][x], config->map)) {
                rendering_map[y][x] = MAP_BLANK;
                continue;
            }

            // remove generation-time object markers
            switch (map->map[y][x]) {
            case MAP_PLAYER:
            case MAP_ENEMY:
            case MAP_ITEM:
                rendering_map[y][x] = MAP_FLOOR;
                break;

            default:
                rendering_map[y][x] = map->map[y][x];
                break;
            }
        }
    }

    // render stair
    for (uint16_t y = 0; y < LOGUE_MAP_HEIGHT; y++) {
        for (uint16_t x = 0; x < LOGUE_MAP_WIDTH; x++) {
            if (map->map[y][x] != MAP_STAIR)
                continue;

            if (is_renderable(map->visibility[y][x], config->stair)) {
                rendering_map[y][x] = MAP_STAIR;
            }
        }
    }

    // render items
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ITEMS; i++) {
        if (!map->items[i].available) continue;

        uint16_t x = map->items[i].x;
        uint16_t y = map->items[i].y;

        if (is_renderable(map->visibility[y][x], config->item)) {
            rendering_map[y][x] = MAP_ITEM;
        }
    }

    // render enemies
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        if (!map->enemies[i].available) continue;

        uint16_t x = map->enemies[i].x;
        uint16_t y = map->enemies[i].y;

        if (is_renderable(map->visibility[y][x], config->enemy)) {
            rendering_map[y][x] = MAP_ENEMY;
        }
    }

    // render players
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_PLAYERS; i++) {
        if (!map->players[i].available) continue;

        uint16_t x = map->players[i].x;
        uint16_t y = map->players[i].y;

        if (is_renderable(map->visibility[y][x], config->player)) {
            rendering_map[y][x] = MAP_PLAYER;
        }
    }

    // print map
    for (uint16_t y = 0; y < LOGUE_MAP_HEIGHT; y++) {
        for (uint16_t x = 0; x < LOGUE_MAP_WIDTH; x++) {
            switch(rendering_map[y][x]) {
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

bool LogueRenderer::is_renderable(uint8_t visibility, uint8_t config) {
    if (config & RENDER_ALWAYS) {
        return true;
    }

    return (visibility & config) != 0;
}