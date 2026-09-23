#include "logue_generator.hpp"

LogueGenerator::LogueGenerator(void) {

}

void LogueGenerator::generate_map(logue_map_t* map, int seed, uint8_t difficult) {
    map->seed = seed;
    map->difficult = difficult;

    srand(map->seed);
    rand();

    // resetting map
    for (uint16_t y = 0; y < LOGUE_MAP_HEIGHT; y++) {
        for (uint16_t x = 0; x < LOGUE_MAP_WIDTH; x++) {
            map->map[y][x] = MAP_BLANK;
        }
    }

    // make rooms
    for (uint16_t i = 0; i < LOGUE_ROOM_NUM; i++) {
        logue_room_t room = {};

        if((rand() % 10) > 7) {
            room.available = 0;
            this->room[i] = room;
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
    
        this->room[i] = room;
        make_room(map, &room);
    }

    // make stair
    while(1) {
        uint16_t x = rand() % LOGUE_MAP_WIDTH;
        uint16_t y = rand() % LOGUE_MAP_HEIGHT;
        if(map->map[y][x] == MAP_FLOOR) {
            map->map[y][x] = MAP_STAIR;
            break;
        }
    }

    // spawn player
    while(1) {
        uint16_t x = rand() % LOGUE_MAP_WIDTH;
        uint16_t y = rand() % LOGUE_MAP_HEIGHT;
        if(map->map[y][x] == MAP_FLOOR) {
            map->map[y][x] = MAP_PLAYER;
            break;
        }
    }

    // make items
    for (uint16_t i = 0; i < 16;) { //test
        uint16_t x = rand() % LOGUE_MAP_WIDTH;
        uint16_t y = rand() % LOGUE_MAP_HEIGHT;
        if(map->map[y][x] == MAP_FLOOR) {
            map->map[y][x] = MAP_ITEM;
            i++;
        }
    }

    // make corridors
    for (uint16_t i = 0; i < LOGUE_ROOM_NUM; i++) {
        if (!this->room[i].available)
            continue;

        logue_room_t *room = &this->room[i];

        // connect to east room
        if ((i % LOGUE_ROOM_COLUMN_NUM) < (LOGUE_ROOM_COLUMN_NUM - 1)) {
            logue_room_t *east = &this->room[i + 1];

            if (east->available) {
                // current room: east wall
                uint16_t x1 = room->x + room->width;
                uint16_t y1 = room->y + 1 + rand() % (room->height - 2);

                // east room: west wall
                uint16_t x2 = east->x;
                uint16_t y2 = east->y + 1 + rand() % (east->height - 2);

                make_corridor(map, x1, y1, x2, y2, CORRIDOR_HORIZONTAL);
            }
        }

        // connect to south room
        if ((i / LOGUE_ROOM_COLUMN_NUM) < (LOGUE_ROOM_ROW_NUM - 1)) {
            logue_room_t *south = &this->room[i + LOGUE_ROOM_COLUMN_NUM];

            if (south->available) {
                // current room: south wall
                uint16_t x1 = room->x + 1 + rand() % (room->width - 2);
                uint16_t y1 = room->y + room->height;

                // south room: north wall
                uint16_t x2 = south->x + 1 + rand() % (south->width - 2);
                uint16_t y2 = south->y;

                make_corridor(map, x1, y1, x2, y2, CORRIDOR_VERTICAL); //test
            }
        }
    }
}

void LogueGenerator::make_room(logue_map_t* map, logue_room_t* room) {
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

void LogueGenerator::make_corridor(logue_map_t *map, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t direction) {
    //test
    map->map[y1][x1] = MAP_ENEMY;
    map->map[y2][x2] = MAP_ENEMY;

    switch (direction)
    {
    case CORRIDOR_HORIZONTAL:   // (-)
        /* code */
        break;
    case CORRIDOR_DIAGONAL_1:   // (/)
        /* code */
        break;
    case CORRIDOR_VERTICAL:     // (|)
        /* code */
        break;
    case CORRIDOR_DIAGONAL_2:   // (\\)
        /* code */
        break;
    default:
        break;
    }
}