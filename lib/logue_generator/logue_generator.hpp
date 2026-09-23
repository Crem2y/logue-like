#pragma once

#include <stdlib.h>
#include <stdint.h>

#include "logue_core.hpp"

#define LOGUE_ROOM_ROW_NUM 3 // -
#define LOGUE_ROOM_COLUMN_NUM 3 // |
#define LOGUE_ROOM_NUM (LOGUE_ROOM_ROW_NUM * LOGUE_ROOM_COLUMN_NUM)

#define LOGUE_ROOM_WIDTH_MAX (LOGUE_MAP_WIDTH  / LOGUE_ROOM_COLUMN_NUM)
#define LOGUE_ROOM_HEIGHT_MAX (LOGUE_MAP_HEIGHT / LOGUE_ROOM_ROW_NUM)

#define LOGUE_ROOM_MIN_SIZE 6

typedef struct {
    uint8_t available;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} logue_room_t;

enum logue_corridor_direction {
    CORRIDOR_HORIZONTAL = 0,    // (-)
    CORRIDOR_DIAGONAL_1,        // (/)
    CORRIDOR_VERTICAL,          // (|)
    CORRIDOR_DIAGONAL_2,        // (\\)
};

class LogueGenerator {
public:
    LogueGenerator(void);

    void generate_map(logue_map_t* map, int seed, uint8_t difficult);
    void make_room(logue_map_t* map, logue_room_t* room);
    void make_corridor(logue_map_t *map, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t direction);

private:
    logue_room_t room[LOGUE_ROOM_NUM];
};