#include "logue_core.hpp"

LogueCore::LogueCore(int seed) {
    core_data.seed = seed;
}

void LogueCore::initialize(void) {
    core_data.floor = 1;   
}

void LogueCore::set_map(logue_map_t* map) {
    memcpy(&floor_info, map, sizeof(logue_map_t));
}

void LogueCore::get_map(logue_map_t* map) {
    memcpy(map, &floor_info, sizeof(logue_map_t));
}

void LogueCore::set_data(logue_data_t* data) {
    memcpy(&core_data, data, sizeof(logue_data_t));
}

void LogueCore::get_data(logue_data_t* data) {
    memcpy(data, &core_data, sizeof(logue_data_t));
}

enum logue_turn_result LogueCore::process_turn(enum logue_cmd cmd) {
    enum logue_turn_result turn_result = TURN_NONE;

    turn_result = update_player(cmd);

    update_visibility();

    if(turn_result != TURN_PROCESSED) return turn_result;

    update_enemies();

    return turn_result;
}

enum logue_turn_result LogueCore::update_player(enum logue_cmd cmd) {
    enum logue_turn_result turn_result = TURN_NONE;

    logue_player_t player = floor_info.players[0];
    bool is_movement_cmd = false;

    int16_t dx = 0;
    int16_t dy = 0;

    switch (cmd) {
    case CMD_MOVE_UP:
        player.direction = DIRECTION_UP;
        is_movement_cmd = true;
        break;
    case CMD_MOVE_DOWN:
        player.direction = DIRECTION_DOWN;
        is_movement_cmd = true;
        break;
    case CMD_MOVE_LEFT:
        player.direction = DIRECTION_LEFT;
        is_movement_cmd = true;
        break;
    case CMD_MOVE_RIGHT:
        player.direction = DIRECTION_RIGHT;
        is_movement_cmd = true;
        break;
    case CMD_ATTACK: {
        enum logue_map_element element = MAP_BLANK;

        direction_to_offset(player.direction, &dx, &dy);
        element = check_position(player.x + dx, player.y + dy);

        switch (element) {
        case MAP_WALL:
            printf("you attacked wall!\n");
            break;
        case MAP_ENEMY:
            printf("you attacked enemy!\n");
            attack_enemy(player.x + dx, player.y + dy);
            break;
        case MAP_ITEM:
            printf("you attacked item!\n");
            break;
        default:
            printf("you attacked nothing!\n");
            break;
        }
        turn_result = TURN_PROCESSED;
        break;
    }
    case CMD_SEARCH: {
        printf("searched!\n");
        enum logue_map_element element = check_position(player.x, player.y);
        if(element == MAP_STAIR) {
            core_data.floor += 1;
            turn_result = TURN_NEXT_FLOOR;
            printf("you moved to the next floor\n");
        } else {
            turn_result = TURN_PROCESSED;
        }
        break;
    }
    case CMD_NONE: {
        turn_result = TURN_PROCESSED;
        break;
    }
    case CMD_NONE_NO_TURN:
    default:
        turn_result = TURN_NONE;
        break;
    }

    // movement
    if (is_movement_cmd) {
        direction_to_offset(player.direction, &dx, &dy);
        enum logue_map_element element = move(&player.x, &player.y, dx, dy);

        switch (element) {
        case MAP_WALL:
            printf("you bumped into wall!\n");
            break;
        case MAP_ENEMY:
            printf("you bumped into enemy!\n");
            break;
        case MAP_ITEM:
            pickup_item(player.x, player.y);
            turn_result = TURN_PROCESSED;
            break;
        default:
            turn_result = TURN_PROCESSED;
            break;
        }
    }

    floor_info.players[0] = player;

    return turn_result;
}

void LogueCore::update_visibility(void) {
    logue_player_t player = floor_info.players[0];

    // update visibility
    for(uint16_t i=0; i<LOGUE_MAP_HEIGHT; i++) {
        for(uint16_t j=0; j<LOGUE_MAP_WIDTH; j++) {
            floor_info.visibility[i][j] &= ~(VIS_VISIBLE | VIS_SEARCHED);
        }
    }

    for (int16_t dy = -1; dy <= 1; dy++) {
        for (int16_t dx = -1; dx <= 1; dx++) {
            int16_t x = player.x + dx;
            int16_t y = player.y + dy;

            if (x < 0 || x >= LOGUE_MAP_WIDTH || y < 0 || y >= LOGUE_MAP_HEIGHT) continue;

            floor_info.visibility[y][x] = (VIS_DISCOVERED | VIS_VISIBLE);
        }
    }

    int16_t room_num = get_room_at(player.x, player.y);
    if(room_num > -1) {
        logue_room_t room = floor_info.room[room_num];
        for(uint16_t i=0; i<room.height+1; i++) {
            for(uint16_t j=0; j<room.width+1; j++) {
                floor_info.visibility[room.y+i][room.x+j] = (VIS_DISCOVERED | VIS_VISIBLE);
            }
        }
    }
}

void LogueCore::update_enemies(void) {
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        logue_enemy_t enemy = floor_info.enemies[i];
        if(!enemy.available) continue;

        uint8_t direction = rand() % 8;
        direction &= 0xFE; // 8-direction to 4-direction
        enemy.direction = direction;

        int16_t dx = 0;
        int16_t dy = 0;

        direction_to_offset(direction, &dx, &dy);
        move(&enemy.x, &enemy.y, dx, dy);
        
        floor_info.enemies[i] = enemy;
    }
}

enum logue_map_element LogueCore::move(uint16_t* x, uint16_t* y, int16_t dx, int16_t dy) {
    int16_t target_x = *x + dx;
    int16_t target_y = *y + dy;

    enum logue_map_element element = check_position(target_x, target_y);

    switch (element) {
    case MAP_WALL:
    case MAP_PLAYER:
    case MAP_ENEMY:
        break;

    default:
        *x = target_x;
        *y = target_y;
        break;
    }

    return element;
}

enum logue_map_element LogueCore::check_position(uint16_t x, uint16_t y) {
    if (x >= LOGUE_MAP_WIDTH || y >= LOGUE_MAP_HEIGHT) {
        return MAP_WALL;
    }

    // check map tile
    if(floor_info.map[y][x] == MAP_WALL || floor_info.map[y][x] == MAP_BLANK) {
        return MAP_WALL;
    }

    // check stair
    if(floor_info.map[y][x] == MAP_STAIR) {
        return MAP_STAIR;
    }

    // check player
    for(uint16_t i = 0; i < LOGUE_MAP_MAX_PLAYERS; i++) {
        if(!floor_info.players[i].available) continue;
        if(floor_info.players[i].x == x && floor_info.players[i].y == y) {
            return MAP_PLAYER;
        }
    }

    // check enemy
    for(uint16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        if(!floor_info.enemies[i].available) continue;
        if(floor_info.enemies[i].x == x && floor_info.enemies[i].y == y) {
            return MAP_ENEMY;
        }
    }

    // check item
    for(uint16_t i = 0; i < LOGUE_MAP_MAX_ITEMS; i++) {
        if(!floor_info.items[i].available) continue;
        if(floor_info.items[i].x == x && floor_info.items[i].y == y) {
            return MAP_ITEM;
        }
    }

    // check other
    if(floor_info.map[y][x] == MAP_FLOOR) {
        return MAP_FLOOR;
    }

    return MAP_FLOOR;
}

int16_t LogueCore::get_room_at(uint16_t x, uint16_t y) {
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ROOMS; i++) {
        logue_room_t* room = &floor_info.room[i];

        if (!room->available) continue;

        if (x > room->x &&
            x < room->x + room->width &&
            y > room->y &&
            y < room->y + room->height) {
            return i;
        }
    }

    return -1;
}

void LogueCore::direction_to_offset(uint8_t direction, int16_t* dx, int16_t* dy) {
    *dx = 0;
    *dy = 0;

    switch (direction) {
    case DIRECTION_UP:         *dy = -1; break;
    case DIRECTION_UP_LEFT:    *dx = -1; *dy = -1; break;
    case DIRECTION_LEFT:       *dx = -1; break;
    case DIRECTION_DOWN_LEFT:  *dx = -1; *dy =  1; break;
    case DIRECTION_DOWN:       *dy =  1; break;
    case DIRECTION_DOWN_RIGHT: *dx =  1; *dy =  1; break;
    case DIRECTION_RIGHT:      *dx =  1; break;
    case DIRECTION_UP_RIGHT:   *dx =  1; *dy = -1; break;
    }
}

void LogueCore::attack_enemy(uint16_t x, uint16_t y) {
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        logue_enemy_t* enemy = &floor_info.enemies[i];

        if (!enemy->available) continue;

        if (enemy->x == x && enemy->y == y) {
            enemy->available = false; //test, hp 1 enemy...
            printf("you defeated the enemy!\n");
            return;
        }
    }
}

void LogueCore::pickup_item(uint16_t x, uint16_t y) {
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ITEMS; i++) {
        logue_item_t* item = &floor_info.items[i];

        if (!item->available) continue;

        if (item->x == x && item->y == y) {
            item->available = false;
            printf("you picked up item!\n");
            return;
        }
    }
}