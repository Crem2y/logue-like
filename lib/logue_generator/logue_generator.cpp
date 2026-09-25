#include "logue_generator.hpp"

LogueGenerator::LogueGenerator(void) {

}

void LogueGenerator::generate_map(logue_map_t* map, int seed, uint8_t difficulty) {
    map->seed = seed;
    map->difficulty = difficulty;

    srand(map->seed);
    rand();

    // resetting map
    reset_map(map);

    // make rooms
    for (uint16_t i = 0; i < LOGUE_ROOM_GEN_NUM; i++) {
        logue_room_t room = {};

        if((rand() % 10) > 7) {
            room.available = false;
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
        room.available = true;
        room.x = x;
        room.y = y;
        room.width = width;
        room.height = height;
    
        map->room[i] = room;
        make_room(map, &room);
    }
    make_corridors(map);
    spawn_stair(map);

    spawn_player(map);
    spawn_enemies(map, LOGUE_MAP_MAX_ENEMIES, difficulty);
    spawn_items(map, LOGUE_MAP_MAX_ITEMS, difficulty);
}

void LogueGenerator::reset_map(logue_map_t* map) {
    for (uint16_t y = 0; y < LOGUE_MAP_HEIGHT; y++) {
        for (uint16_t x = 0; x < LOGUE_MAP_WIDTH; x++) {
            map->map[y][x] = MAP_BLANK;
            map->visibility[y][x] = 0;
        }
    }
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ROOMS; i++) {
        map->room[i] = {};
    }
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_PLAYERS; i++) {
        map->players[i] = {};
    }
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        map->enemies[i] = {};
    }
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ITEMS; i++) {
        map->items[i] = {};
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

void LogueGenerator::make_corridors(logue_map_t* map) {
    for (uint16_t i = 0; i < LOGUE_ROOM_GEN_NUM; i++) {
        if (!map->room[i].available)
            continue;

        logue_room_t *room = &map->room[i];

        // connect to east room
        if ((i % LOGUE_ROOM_COLUMN_NUM) < (LOGUE_ROOM_COLUMN_NUM - 1)) {
            logue_room_t *east = &map->room[i + 1];

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
            logue_room_t *south = &map->room[i + LOGUE_ROOM_COLUMN_NUM];

            if (south->available) {
                // current room: south wall
                uint16_t x1 = room->x + 1 + rand() % (room->width - 2);
                uint16_t y1 = room->y + room->height;

                // south room: north wall
                uint16_t x2 = south->x + 1 + rand() % (south->width - 2);
                uint16_t y2 = south->y;

                make_corridor(map, x1, y1, x2, y2, CORRIDOR_VERTICAL);
            }
        }
    }
}

void LogueGenerator::make_corridor(logue_map_t *map, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t direction) {
    switch (direction)
    {
    case CORRIDOR_HORIZONTAL: { // (-)
        uint16_t turn_x = x1 + 1 + rand() % (x2 - x1 - 1);

        // first room -> turn point
        for (uint16_t x = x1; x <= turn_x; x++) {
            map->map[y1][x] = MAP_FLOOR;
        }

        // vertical section
        if (y1 < y2) {
            for (uint16_t y = y1; y <= y2; y++) {
                map->map[y][turn_x] = MAP_FLOOR;
            }
        }
        else {
            for (uint16_t y = y2; y <= y1; y++) {
                map->map[y][turn_x] = MAP_FLOOR;
            }
        }

        // turn point -> second room
        for (uint16_t x = turn_x; x <= x2; x++) {
            map->map[y2][x] = MAP_FLOOR;
        }

        break;
    }

    case CORRIDOR_VERTICAL: { // (|)
        uint16_t turn_y = y1 + 1 + rand() % (y2 - y1 - 1);

        // first room -> turn point
        for (uint16_t y = y1; y <= turn_y; y++) {
            map->map[y][x1] = MAP_FLOOR;
        }

        // horizontal section
        if (x1 < x2) {
            for (uint16_t x = x1; x <= x2; x++) {
                map->map[turn_y][x] = MAP_FLOOR;
            }
        }
        else {
            for (uint16_t x = x2; x <= x1; x++) {
                map->map[turn_y][x] = MAP_FLOOR;
            }
        }

        // turn point -> second room
        for (uint16_t y = turn_y; y <= y2; y++) {
            map->map[y][x2] = MAP_FLOOR;
        }

        break;
    }
    case CORRIDOR_DIAGONAL_1:   // (/)
        /* code */
        break;
    case CORRIDOR_DIAGONAL_2:   // (\\)
        /* code */
        break;
    default:
        break;
    }
}

void LogueGenerator::spawn_stair(logue_map_t* map) {
    uint16_t x;
    uint16_t y;

    if (get_random_empty_position(map, &x, &y)) {
        map->map[y][x] = MAP_STAIR;
    }
}

void LogueGenerator::spawn_player(logue_map_t* map) {
    uint16_t x;
    uint16_t y;

    if (get_random_empty_position(map, &x, &y)) {
        map->map[y][x] = MAP_PLAYER;
        map->players[0].available = true;
        map->players[0].x = x;
        map->players[0].y = y;
    }
}

void LogueGenerator::spawn_enemies(logue_map_t* map, uint8_t num, uint8_t difficulty) {
    for (uint16_t i = 0; i < num; i++) {
        map->enemies[i] = spawn_enemy(map, difficulty);
    }
}

logue_enemy_t LogueGenerator::spawn_enemy(logue_map_t* map, uint8_t difficulty) {
    logue_enemy_t enemy = {};

    uint16_t x;
    uint16_t y;

    if (!get_random_empty_position(map, &x, &y)) {
        return enemy;
    }

    map->map[y][x] = MAP_ENEMY;

    enemy.available = true;
    enemy.x = x;
    enemy.y = y;

    // todo: generating enemy info...

    return enemy;
}

void LogueGenerator::spawn_items(logue_map_t* map, uint8_t num, uint8_t difficulty) {
    for (uint16_t i = 0; i < num; i++) {
        map->items[i] = spawn_item(map, difficulty);
    }
}

logue_item_t LogueGenerator::spawn_item(logue_map_t* map, uint8_t difficulty) {
    logue_item_t item = {};

    uint16_t x;
    uint16_t y;

    if (!get_random_empty_position(map, &x, &y)) {
        return item;
    }

    map->map[y][x] = MAP_ITEM;

    item.available = true;
    item.x = x;
    item.y = y;

    // todo: generating item info...

    return item;
}

bool LogueGenerator::get_random_empty_position(logue_map_t* map, uint16_t* out_x, uint16_t* out_y) {
    uint32_t empty_count = 0;

    // count empty positions
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ROOMS; i++) {
        logue_room_t* room = &map->room[i];

        if (!room->available) continue;

        for (uint16_t y = room->y + 1; y < room->y + room->height; y++) {
            for (uint16_t x = room->x + 1; x < room->x + room->width; x++) {
                if (map->map[y][x] == MAP_FLOOR) {
                    empty_count++;
                }
            }
        }
    }

    if (empty_count == 0) {
        return false;
    }

    // select random empty position
    uint32_t target = rand() % empty_count;

    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ROOMS; i++) {
        logue_room_t* room = &map->room[i];

        if (!room->available) continue;

        for (uint16_t y = room->y + 1; y < room->y + room->height; y++) {
            for (uint16_t x = room->x + 1; x < room->x + room->width; x++) {
                if (map->map[y][x] != MAP_FLOOR) continue;

                if (target == 0) {
                    *out_x = x;
                    *out_y = y;
                    return true;
                }

                target--;
            }
        }
    }

    return false;
}