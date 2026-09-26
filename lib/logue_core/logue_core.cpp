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

    if(turn_result != TURN_PROCESSED) {
        update_visibility();
        return turn_result;
    }

    update_enemies();
    update_visibility();

    turn_result = check_game_state();
    if(turn_result != TURN_PROCESSED) {
        return turn_result;
    }

    if (cmd == CMD_SEARCH) {
        search();
    }

    return turn_result;
}

enum logue_turn_result LogueCore::update_player(enum logue_cmd cmd) {
    enum logue_turn_result turn_result = TURN_NONE;

    logue_player_t* player = &floor_info.players[0];
    bool is_movement_cmd = false;

    int16_t dx = 0;
    int16_t dy = 0;

    switch (cmd) {
    case CMD_MOVE_UP:
        player->direction = DIRECTION_UP;
        is_movement_cmd = true;
        break;
    case CMD_MOVE_DOWN:
        player->direction = DIRECTION_DOWN;
        is_movement_cmd = true;
        break;
    case CMD_MOVE_LEFT:
        player->direction = DIRECTION_LEFT;
        is_movement_cmd = true;
        break;
    case CMD_MOVE_RIGHT:
        player->direction = DIRECTION_RIGHT;
        is_movement_cmd = true;
        break;
    case CMD_ATTACK: {
        direction_to_offset(player->direction, &dx, &dy);
        int16_t target_x = player->x + dx;
        int16_t target_y = player->y + dy;

        enum logue_map_element terrain = get_terrain_at(target_x, target_y);
        logue_object_ref_t object = get_object_at(target_x, target_y);

        if (terrain == MAP_WALL) {
            printf("you attacked a wall!\n");
        } else if (object.type == MAP_ENEMY) {
            //printf("you attacked an enemy!\n");
            attack((logue_object_ref_t){MAP_PLAYER, 0}, object);
        } else if (object.type == MAP_ITEM) {
            printf("you attacked an item!\n");
        } else {
            printf("you attacked nothing!\n");
        }
        turn_result = TURN_PROCESSED;
        break;
    }
    case CMD_SEARCH: {
        enum logue_map_element element = get_terrain_at(player->x, player->y);
        if(element == MAP_STAIR) {
            core_data.floor += 1;
            turn_result = TURN_NEXT_FLOOR;
            printf("you moved to the next floor\n");
        } else {
            turn_result = TURN_PROCESSED;
            printf("searching...\n");
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
        direction_to_offset(player->direction, &dx, &dy);
        enum logue_map_element element = move(&player->x, &player->y, dx, dy);

        switch (element) {
        case MAP_WALL:
            printf("you bumped into a wall!\n");
            break;
        case MAP_ENEMY:
            printf("you bumped into an enemy!\n");
            break;
        case MAP_ITEM:
            pickup_item_at(player->x, player->y);
            turn_result = TURN_PROCESSED;
            break;
        default:
            turn_result = TURN_PROCESSED;
            break;
        }
    }

    return turn_result;
}

void LogueCore::update_visibility(void) {
    logue_player_t* player = &floor_info.players[0];

    // update visibility
    for(uint16_t i=0; i<LOGUE_MAP_HEIGHT; i++) {
        for(uint16_t j=0; j<LOGUE_MAP_WIDTH; j++) {
            floor_info.visibility[i][j] &= ~(VIS_VISIBLE | VIS_SEARCHED);
        }
    }

    for (int16_t dy = -1; dy <= 1; dy++) {
        for (int16_t dx = -1; dx <= 1; dx++) {
            int16_t x = player->x + dx;
            int16_t y = player->y + dy;

            if (x < 0 || x >= LOGUE_MAP_WIDTH || y < 0 || y >= LOGUE_MAP_HEIGHT) continue;

            floor_info.visibility[y][x] |= (VIS_DISCOVERED | VIS_VISIBLE);
        }
    }

    int16_t room_num = get_room_at(player->x, player->y);
    if(room_num > -1) {
        logue_room_t room = floor_info.room[room_num];
        for(uint16_t i=0; i<room.height+1; i++) {
            for(uint16_t j=0; j<room.width+1; j++) {
                floor_info.visibility[room.y+i][room.x+j] |= (VIS_DISCOVERED | VIS_VISIBLE);
            }
        }
    }
}

void LogueCore::update_enemies(void) {
    for (int16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        logue_enemy_t* enemy = &floor_info.enemies[i];
        if(!enemy->available) continue;

        uint8_t direction = rand() % 8;
        direction &= 0xFE; // 8-direction to 4-direction
        enemy->direction = direction;

        int16_t dx = 0;
        int16_t dy = 0;

        direction_to_offset(direction, &dx, &dy);
        enum logue_map_element element = move(&enemy->x, &enemy->y, dx, dy);

        //test
        if(element == MAP_PLAYER) {
            attack((logue_object_ref_t){MAP_ENEMY, i}, (logue_object_ref_t){MAP_PLAYER, 0});
        }
    }
}

enum logue_turn_result LogueCore::check_game_state(void) {
    enum logue_turn_result turn_result = TURN_PROCESSED;

    logue_player_t* player = &floor_info.players[0];

    if (!player->available) {
        return TURN_GAME_OVER;
    }

    return turn_result;
}

enum logue_map_element LogueCore::move(uint16_t* x, uint16_t* y, int16_t dx, int16_t dy) {
    int16_t target_x = *x + dx;
    int16_t target_y = *y + dy;

    enum logue_map_element terrain = get_terrain_at(target_x, target_y);

    if (terrain == MAP_WALL) return MAP_WALL;

    logue_object_ref_t object = get_object_at(target_x, target_y);

    switch (object.type) {
    case MAP_PLAYER:
    case MAP_ENEMY:
        return object.type;

    default:
        break;
    }

    *x = target_x;
    *y = target_y;

    if (object.type != MAP_BLANK) return object.type;

    return terrain;
}

enum logue_map_element LogueCore::get_terrain_at(uint16_t x, uint16_t y) {
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

    // check other
    if(floor_info.map[y][x] == MAP_FLOOR) {
        return MAP_FLOOR;
    }

    return MAP_FLOOR;
}

logue_object_ref_t LogueCore::get_object_at(uint16_t x, uint16_t y) {
    logue_object_ref_t object = {MAP_BLANK, -1};

    for (int16_t i = 0; i < LOGUE_MAP_MAX_PLAYERS; i++) {
        logue_player_t* player = &floor_info.players[i];
        if (!player->available) continue;

        if (player->x == x && player->y == y) {
            object = {MAP_PLAYER, i};
            return object;
        }
    }

    for (int16_t i = 0; i < LOGUE_MAP_MAX_ENEMIES; i++) {
        logue_enemy_t* enemy = &floor_info.enemies[i];
        if (!enemy->available) continue;

        if (enemy->x == x && enemy->y == y) {
            object = {MAP_ENEMY, i};
            return object;
        }
    }

    for (int16_t i = 0; i < LOGUE_MAP_MAX_ITEMS; i++) {
        logue_item_t* item = &floor_info.items[i];
        if (!item->available) continue;

        if (item->x == x && item->y == y) {
            object = {MAP_ITEM, i};
            return object;
        }
    }

    return object;
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

void LogueCore::attack(logue_object_ref_t attacker, logue_object_ref_t target) {
    int16_t damage = 0;

    switch (attacker.type) {
    case MAP_PLAYER:
        if (attacker.index < 0 || attacker.index >= LOGUE_MAP_MAX_PLAYERS) return;

        damage = floor_info.players[attacker.index].power;
        break;

    case MAP_ENEMY:
        if (attacker.index < 0 || attacker.index >= LOGUE_MAP_MAX_ENEMIES) return;

        damage = floor_info.enemies[attacker.index].power;
        break;
    }

    switch (target.type) {
    case MAP_PLAYER: {
        if (target.index < 0 || target.index >= LOGUE_MAP_MAX_PLAYERS) return;

        logue_player_t* player = &floor_info.players[target.index];

        player->hp -= damage;
        printf("you were attacked! (%d)\n", -damage);

        if (player->hp <= 0) {
            player->hp = 0;
            player->available = false;
            printf("you died\n");
        }
        break;
    }
    case MAP_ENEMY: {
        if (target.index < 0 || target.index >= LOGUE_MAP_MAX_ENEMIES) return;

        logue_enemy_t* enemy = &floor_info.enemies[target.index];

        enemy->hp -= damage;
        printf("you attacked an enemy! (%d)\n", -damage);

        if (enemy->hp <= 0) {
            enemy->hp = 0;
            enemy->available = false;
            printf("you defeated the enemy!\n");
        }
        break;
    }
    }
}

void LogueCore::pickup_item_at(uint16_t x, uint16_t y) {
    for (uint16_t i = 0; i < LOGUE_MAP_MAX_ITEMS; i++) {
        logue_item_t* item = &floor_info.items[i];

        if (!item->available) continue;

        if (item->x == x && item->y == y) {
            item->available = false;
            printf("you picked up an item!\n");
            return;
        }
    }
}

void LogueCore::search(void) {
    logue_player_t* player = &floor_info.players[0];

    for (int16_t dy = -1; dy <= 1; dy++) {
        for (int16_t dx = -1; dx <= 1; dx++) {
            int16_t x = player->x + dx;
            int16_t y = player->y + dy;

            if (x < 0 || x >= LOGUE_MAP_WIDTH || y < 0 || y >= LOGUE_MAP_HEIGHT) continue;

            floor_info.visibility[y][x] |= VIS_SEARCHED;
        }
    }
    printf("search complete!\n");
}