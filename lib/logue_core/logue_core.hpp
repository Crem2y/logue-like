#pragma once

#include <stdlib.h>
#include <stdint.h>

#define LOGUE_MAP_WIDTH 60
#define LOGUE_MAP_HEIGHT 60

#define LOGUE_ROOM_ROW_NUM 3 // -
#define LOGUE_ROOM_COLUMN_NUM 3 // |
#define LOGUE_ROOM_NUM (LOGUE_ROOM_ROW_NUM * LOGUE_ROOM_COLUMN_NUM)

#define LOGUE_ROOM_WIDTH_MAX (LOGUE_MAP_WIDTH  / LOGUE_ROOM_COLUMN_NUM)
#define LOGUE_ROOM_HEIGHT_MAX (LOGUE_MAP_HEIGHT / LOGUE_ROOM_ROW_NUM)

#define LOGUE_ROOM_MIN_SIZE 6

typedef struct {
    int seed;
} logue_data_t;

typedef struct {
    uint8_t available;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} logue_room_t;

typedef struct {
    int seed;
    uint8_t difficult;
    uint8_t map[LOGUE_MAP_HEIGHT][LOGUE_MAP_WIDTH];
    logue_room_t room[LOGUE_ROOM_NUM];
} logue_map_t;

enum logue_map_element {
    MAP_BLANK = 0,
    MAP_FLOOR,
    MAP_WALL,
    MAP_STAIR,
    MAP_PLAYER,
    MAP_ENEMY,
    MAP_ITEM,
};

enum logue_corridor_direction {
    CORRIDOR_HORIZONTAL = 0,    // (-)
    CORRIDOR_DIAGONAL_1,        // (/)
    CORRIDOR_VERTICAL,          // (|)
    CORRIDOR_DIAGONAL_2,        // (\\)
};

class LogueCore {
public:
    LogueCore(int seed);

    void initialize(void);
    void generate_map(logue_map_t* map, int seed, uint8_t difficult);
    void make_room(logue_map_t* map, logue_room_t* room);
    void make_corridor(logue_map_t *map, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t direction);

    int get_seed(void) { return core_data.seed; }
    int get_map_seed(void) { return map.seed; }
    uint8_t get_map_difficult(void) { return map.difficult; }

private:
    logue_data_t core_data;
    logue_map_t map;
};