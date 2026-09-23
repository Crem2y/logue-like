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
        logue_room_t room = {};

        if((rand() % 10) > 7) {
            room.available = 0;
            map->room[i] = room;
            continue;
        }

        uint16_t column = i % LOGUE_ROOM_COLUMN_NUM;
        uint16_t row    = i / LOGUE_ROOM_COLUMN_NUM;

        uint16_t cell_x = column * LOGUE_ROOM_WIDTH_MAX;
        uint16_t cell_y = row * LOGUE_ROOM_HEIGHT_MAX;

        uint16_t max_width  = LOGUE_ROOM_WIDTH_MAX  - 2;
        uint16_t max_height = LOGUE_ROOM_HEIGHT_MAX - 2;

        uint16_t width  = LOGUE_ROOM_MIN_SIZE + rand() % (max_width  - LOGUE_ROOM_MIN_SIZE + 1);
        uint16_t height = LOGUE_ROOM_MIN_SIZE + rand() % (max_height - LOGUE_ROOM_MIN_SIZE + 1);

        uint16_t x = cell_x + 1 + rand() % (LOGUE_ROOM_WIDTH_MAX - width - 1);
        uint16_t y = cell_y + 1 + rand() % (LOGUE_ROOM_HEIGHT_MAX - height - 1);

        // make room
        room.available = 1;
        room.x = x;
        room.y = y;
        room.width = width;
        room.height = height;
    
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