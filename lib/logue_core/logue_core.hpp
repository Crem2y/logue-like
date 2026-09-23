#pragma once

#include <stdlib.h>
#include <stdint.h>

#define LOGUE_MAP_WIDTH 60
#define LOGUE_MAP_HEIGHT 60

#define LOGUE_MAP_MAX_PLAYERS 1
#define LOGUE_MAP_MAX_ENEMIES 16
#define LOGUE_MAP_MAX_ITEMS 16

typedef struct {
    int seed;
} logue_data_t;

typedef struct {
    bool available;
    uint16_t x;
    uint16_t y;
} logue_player_t;

typedef struct {
    bool available;
    uint16_t x;
    uint16_t y;
} logue_enemy_t;

typedef struct {
    bool available;
    uint16_t x;
    uint16_t y;
} logue_item_t;

typedef struct {
    int seed;
    uint8_t difficulty;
    uint8_t map[LOGUE_MAP_HEIGHT][LOGUE_MAP_WIDTH];
    logue_player_t players[LOGUE_MAP_MAX_PLAYERS];
    logue_enemy_t enemies[LOGUE_MAP_MAX_ENEMIES];
    logue_item_t items[LOGUE_MAP_MAX_ITEMS];
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

class LogueCore {
public:
    LogueCore(int seed);

    void initialize(void);

    int get_seed(void) { return core_data.seed; }

private:
    logue_data_t core_data;
};