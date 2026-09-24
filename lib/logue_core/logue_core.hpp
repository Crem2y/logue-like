#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define LOGUE_MAP_WIDTH 30
#define LOGUE_MAP_HEIGHT 30

#define LOGUE_MAP_MAX_ROOMS 16

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
    uint16_t width;
    uint16_t height;
} logue_room_t;

typedef struct {
    bool available;
    uint8_t direction;
    uint16_t x;
    uint16_t y;
} logue_player_t;

typedef struct {
    bool available;
    uint8_t direction;
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
    uint8_t visibility[LOGUE_MAP_HEIGHT][LOGUE_MAP_WIDTH];
    logue_room_t room[LOGUE_MAP_MAX_ROOMS];
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

enum logue_direction {
    DIRECTION_UP = 0,
    DIRECTION_UP_LEFT,
    DIRECTION_LEFT,
    DIRECTION_DOWN_LEFT,
    DIRECTION_DOWN,
    DIRECTION_DOWN_RIGHT,
    DIRECTION_RIGHT,
    DIRECTION_UP_RIGHT,
};

enum logue_visibility {
    VIS_DISCOVERED = 0x01,
    VIS_VISIBLE    = 0x02,
    VIS_SEARCHED   = 0x04,
};

enum logue_cmd {
    CMD_NONE = 0,
    CMD_NONE_NO_TURN,
    CMD_MOVE_UP,
    CMD_MOVE_UP_LEFT,
    CMD_MOVE_LEFT,
    CMD_MOVE_DOWN_LEFT,
    CMD_MOVE_DOWN,
    CMD_MOVE_DOWN_RIGHT,
    CMD_MOVE_RIGHT,
    CMD_MOVE_UP_RIGHT,
    CMD_ATTACK,
    CMD_SEARCH,
    CMD_USE,
};

class LogueCore {
public:
    LogueCore(int seed);

    void initialize(void);
    void set_map(logue_map_t* map);
    void get_map(logue_map_t* map);

    void process_turn(enum logue_cmd cmd);
    bool update_player(enum logue_cmd cmd);
    void update_visibility(void);
    void update_enemies(void);
    enum logue_map_element check_position(uint16_t x, uint16_t y);
    enum logue_map_element move(uint16_t* x, uint16_t* y, int16_t dx, int16_t dy);

    int16_t get_room_at(uint16_t x, uint16_t y);

    int get_seed(void) { return core_data.seed; }

private:
    logue_data_t core_data;
    logue_map_t floor_info;
};