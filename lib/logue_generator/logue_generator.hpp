#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "logue_core.hpp"

#define LOGUE_ROOM_ROW_NUM 3 // -
#define LOGUE_ROOM_COLUMN_NUM 3 // |
#define LOGUE_ROOM_GEN_NUM (LOGUE_ROOM_ROW_NUM * LOGUE_ROOM_COLUMN_NUM)
#define LOGUE_ROOM_MIN_NUM 4

#define LOGUE_ROOM_WIDTH_MAX (LOGUE_MAP_WIDTH  / LOGUE_ROOM_COLUMN_NUM)
#define LOGUE_ROOM_HEIGHT_MAX (LOGUE_MAP_HEIGHT / LOGUE_ROOM_ROW_NUM)

#define LOGUE_ROOM_MIN_SIZE 6

enum logue_corridor_direction {
    CORRIDOR_HORIZONTAL = 0,    // (-)
    CORRIDOR_DIAGONAL_1,        // (/)
    CORRIDOR_VERTICAL,          // (|)
    CORRIDOR_DIAGONAL_2,        // (\\)
};

class LogueGenerator {
public:
    LogueGenerator(void);

    void generate_map(logue_map_t* map, int seed, uint8_t difficulty);
    void reset_map(logue_map_t* map);
    void make_rooms(logue_map_t* map);
    void make_room(logue_map_t* map, logue_room_t* room);
    void make_corridors(logue_map_t* map);
    void make_corridor(logue_map_t *map, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t direction);
    void validate_rooms(logue_map_t* map);
    bool is_valid_map(logue_map_t* map);

    void spawn_stair(logue_map_t* map);
    void spawn_player(logue_map_t* map);
    void spawn_enemies(logue_map_t* map, uint8_t num, uint8_t difficulty);
    logue_enemy_t spawn_enemy(logue_map_t* map, uint8_t difficulty);
    void spawn_items(logue_map_t* map, uint8_t num, uint8_t difficulty);
    logue_item_t spawn_item(logue_map_t* map, uint8_t difficulty);

    bool get_random_empty_position(logue_map_t* map, uint16_t* out_x, uint16_t* out_y);

private:
    logue_room_t room[LOGUE_ROOM_GEN_NUM];
    bool room_connection[LOGUE_ROOM_GEN_NUM][LOGUE_ROOM_GEN_NUM];
};