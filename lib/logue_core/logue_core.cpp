#include "logue_core.hpp"

LogueCore::LogueCore(int seed) {
    core_data.seed = seed;
}

void LogueCore::initialize(void) {

}

void LogueCore::generate_map(logue_map_t* map, int seed, uint8_t difficult) {
    map->seed = seed;
    map->difficult = difficult;

    srand(map->seed);
    rand();

    // resetting map
    for (uint16_t y = 0; y < LOGUE_MAP_WIDTH; y++) {
        for (uint16_t x = 0; x < LOGUE_MAP_HEIGHT; x++) {
            map->map[y][x] = MAP_BLANK;
        }
    }

    // make rooms
    for (uint16_t i = 0; i < LOGUE_ROOM_NUM; i++) {
        uint16_t x = rand() % (LOGUE_MAP_WIDTH / 2);
        uint16_t y = rand() % (LOGUE_MAP_HEIGHT / 2);
        uint16_t width = rand() % (LOGUE_MAP_WIDTH / 2);
        uint16_t height = rand() % (LOGUE_MAP_HEIGHT / 2);

        logue_room_t room = {.x=x, .y=y, .width=width, .height=height};
        map->room[i] = room;
        make_room(map, &room);
    }
}

void LogueCore::make_room(logue_map_t* map, logue_room_t* room) {
    // filling floor
    for (uint16_t y = 0; y < room->height; y++) {
        for (uint16_t x = 0; x < room->width; x++) {
            map->map[room->y + y][room->x + x] = MAP_FLOOR;
        }
    }

    // making walls
    for (uint16_t y = 0; y < room->height + 1; y++) {
        map->map[room->y + y][room->x] = MAP_WALL;
        map->map[room->y + y][room->x + room->width] = MAP_WALL;
    }
    for (uint16_t x = 0; x < room->width + 1; x++) {
        map->map[room->y][room->x + x] = MAP_WALL;
        map->map[room->y + room->height][room->x + x] = MAP_WALL;
    }
}