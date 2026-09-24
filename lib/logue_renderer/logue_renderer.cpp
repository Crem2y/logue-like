#include "logue_renderer.hpp"

LogueRenderer::LogueRenderer() {

}

void LogueRenderer::render_map(logue_map_t* map, logue_renderer_config_t* config) {
    printf("map seed = %d, map difficulty = %d\n", map->seed, map->difficulty);

    uint8_t rendering_map[LOGUE_MAP_HEIGHT][LOGUE_MAP_WIDTH];

    // copy map & remove objects
    for (uint16_t y = 0; y < LOGUE_MAP_HEIGHT; y++) {
        for (uint16_t x = 0; x < LOGUE_MAP_WIDTH; x++) {
            if(map->discovered[y][x] == 0x00) {
                rendering_map[y][x] = MAP_BLANK;
                continue;
            }

            if(map->map[y][x] == MAP_PLAYER ||
                map->map[y][x] == MAP_ENEMY ||
                map->map[y][x] == MAP_ITEM) {
                
                rendering_map[y][x] = MAP_FLOOR;
            } else {
                rendering_map[y][x] = map->map[y][x];
            }
        }
    }

    // check item
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ITEMS; i++) {
        if(map->items[i].available) {
            rendering_map[map->items[i].y][map->items[i].x] = MAP_ITEM;
        }
    }

    // check enemy
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        if(map->enemies[i].available) {
            rendering_map[map->enemies[i].y][map->enemies[i].x] = MAP_ENEMY;
        }
    }

    
    // check player
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_PLAYERS; i++) {
        if(map->players[i].available) {
            rendering_map[map->players[i].y][map->players[i].x] = MAP_PLAYER;
        }
    }

    // rendering map
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