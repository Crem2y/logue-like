#include "logue_core.hpp"

LogueCore::LogueCore(int seed) {
    core_data.seed = seed;
}

void LogueCore::initialize(void) {

}

void LogueCore::set_map(logue_map_t* map) {
    memcpy(&floor_info, map, sizeof(logue_map_t));
}

void LogueCore::get_map(logue_map_t* map) {
    memcpy(map, &floor_info, sizeof(logue_map_t));
}

void LogueCore::process_turn(enum logue_cmd cmd) {
    bool is_turn_processed = false;

    logue_player_t player = floor_info.players[0];

    // cmd process
    switch (cmd) {
    case CMD_MOVE_UP: {
        player.direction = DIRECTION_UP;
        enum logue_map_element element = check_position(player.x, player.y-1);
        switch (element) {
        case MAP_WALL:
            printf("wall!\n");
            break;
        case MAP_ENEMY:
            printf("enemy!\n");
            break;
        default:
            player.y -= 1;
            is_turn_processed = true;
            break;
        }
        break;
    }
    case CMD_MOVE_DOWN: {
        player.direction = DIRECTION_DOWN;
        enum logue_map_element element = check_position(player.x, player.y+1);
        switch (element) {
        case MAP_WALL:
            printf("wall!\n");
            break;
        case MAP_ENEMY:
            printf("enemy!\n");
            break;
        default:
            player.y += 1;
            is_turn_processed = true;
            break;
        }
        break;
    }
    case CMD_MOVE_LEFT: {
        player.direction = DIRECTION_LEFT;
        enum logue_map_element element = check_position(player.x-1, player.y);
        switch (element) {
        case MAP_WALL:
            printf("wall!\n");
            break;
        case MAP_ENEMY:
            printf("enemy!\n");
            break;
        default:
            player.x -= 1;
            is_turn_processed = true;
            break;
        }
        break;
    }
    case CMD_MOVE_RIGHT: {
        player.direction = DIRECTION_RIGHT;
        enum logue_map_element element = check_position(player.x+1, player.y);
        switch (element) {
        case MAP_WALL:
            printf("wall!\n");
            break;
        case MAP_ENEMY:
            printf("enemy!\n");
            break;
        default:
            player.x += 1;
            is_turn_processed = true;
            break;
        }
        break;
    }
    case CMD_ATTACK: {
        enum logue_map_element element;
        switch(player.direction) {
        case DIRECTION_UP:
            element = check_position(player.x, player.y-1);
            break;
        case DIRECTION_DOWN:
            element = check_position(player.x, player.y+1);
            break;
        case DIRECTION_LEFT:
            element = check_position(player.x-1, player.y);
            break;
        case DIRECTION_RIGHT:
            element = check_position(player.x+1, player.y);
            break;
        default:
            break;
        }

        switch (element) {
        case MAP_WALL:
            printf("you attacked wall!\n");
            break;
        case MAP_ENEMY:
            printf("you attacked enemy!\n");
            break;
        default:
            printf("you attacked nothing!\n");
            break;
        }
        is_turn_processed = true;
        break;
    }
    case CMD_SEARCH: {
        printf("searched!\n");
        is_turn_processed = true;
        break;
    }
    default:
        is_turn_processed = true;
        break;
    }

    floor_info.players[0] = player;
    if(!is_turn_processed) return;

    // update enemies
    

    // update visibility

}

enum logue_map_element LogueCore::check_position(uint16_t x, uint16_t y) {
    if (x >= LOGUE_MAP_WIDTH || y >= LOGUE_MAP_HEIGHT) {
        return MAP_WALL;
    }

    // check map tile
    if(floor_info.map[y][x] == MAP_WALL || floor_info.map[y][x] == MAP_BLANK) {
        return MAP_WALL;
    }

    // check player
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_PLAYERS; i++) {
        if(!floor_info.players[i].available) continue;
        if(floor_info.players[i].x == x && floor_info.players[i].y == y) {
            return MAP_PLAYER;
        }
    }

    // check enemy
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        if(!floor_info.enemies[i].available) continue;
        if(floor_info.enemies[i].x == x && floor_info.enemies[i].y == y) {
            return MAP_ENEMY;
        }
    }

    // check item
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ITEMS; i++) {
        if(!floor_info.items[i].available) continue;
        if(floor_info.items[i].x == x && floor_info.items[i].y == y) {
            return MAP_ITEM;
        }
    }

    // check other
    if(floor_info.map[y][x] == MAP_STAIR) {
        return MAP_STAIR;
    }
    if(floor_info.map[y][x] == MAP_FLOOR) {
        return MAP_FLOOR;
    }

    return MAP_FLOOR;
}