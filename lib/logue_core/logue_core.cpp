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

    is_turn_processed = update_player(cmd);

    update_visibility();

    if(!is_turn_processed) return;

    update_enemies();
}

bool LogueCore::update_player(enum logue_cmd cmd) {
    bool is_turn_processed = false;

    logue_player_t player = floor_info.players[0];

    // cmd process
    switch (cmd) {
    case CMD_MOVE_UP: {
        player.direction = DIRECTION_UP;
        enum logue_map_element element = move(&player.x, &player.y, 0, -1);
        switch (element) {
        case MAP_WALL:
            printf("wall!\n");
            break;
        case MAP_ENEMY:
            printf("enemy!\n");
            break;
        default:
            is_turn_processed = true;
            break;
        }
        break;
    }
    case CMD_MOVE_DOWN: {
        player.direction = DIRECTION_DOWN;
        enum logue_map_element element = move(&player.x, &player.y, 0, 1);
        switch (element) {
        case MAP_WALL:
            printf("wall!\n");
            break;
        case MAP_ENEMY:
            printf("enemy!\n");
            break;
        default:
            is_turn_processed = true;
            break;
        }
        break;
    }
    case CMD_MOVE_LEFT: {
        player.direction = DIRECTION_LEFT;
        enum logue_map_element element = move(&player.x, &player.y, -1, 0);
        switch (element) {
        case MAP_WALL:
            printf("wall!\n");
            break;
        case MAP_ENEMY:
            printf("enemy!\n");
            break;
        default:
            is_turn_processed = true;
            break;
        }
        break;
    }
    case CMD_MOVE_RIGHT: {
        player.direction = DIRECTION_RIGHT;
        enum logue_map_element element = move(&player.x, &player.y, 1, 0);
        switch (element) {
        case MAP_WALL:
            printf("wall!\n");
            break;
        case MAP_ENEMY:
            printf("enemy!\n");
            break;
        default:
            is_turn_processed = true;
            break;
        }
        break;
    }
    case CMD_ATTACK: {
        enum logue_map_element element = MAP_BLANK;
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
    case CMD_NONE: {
        is_turn_processed = true;
        break;
    }
    case CMD_NONE_NO_TURN:
    default:
        is_turn_processed = false;
        break;
    }

    floor_info.players[0] = player;

    return is_turn_processed;
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

        switch (direction) {
        case DIRECTION_UP:
            move(&enemy.x, &enemy.y, 0, -1);
            break;
        case DIRECTION_DOWN:
            move(&enemy.x, &enemy.y, 0, 1);
            break;
        case DIRECTION_LEFT:
            move(&enemy.x, &enemy.y, -1, 0);
            break;
        case DIRECTION_RIGHT:
            move(&enemy.x, &enemy.y, 1, 0);
            break;
        default:
            break;
        }
        
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
    if(floor_info.map[y][x] == MAP_STAIR) {
        return MAP_STAIR;
    }
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