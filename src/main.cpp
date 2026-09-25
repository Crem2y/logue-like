#include "main.hpp"

const logue_renderer_config_t normal_config = {
    .map    = RENDER_DISCOVERED,
    .stair  = RENDER_DISCOVERED,
    .player = RENDER_SEARCHED,
    .enemy  = RENDER_SEARCHED,
    .item   = RENDER_DISCOVERED,
};

const logue_renderer_config_t insight_config = {
    .map    = RENDER_DISCOVERED,
    .stair  = RENDER_DISCOVERED,
    .player = RENDER_ALWAYS,
    .enemy  = RENDER_VISIBLE,
    .item   = RENDER_DISCOVERED,
};

const logue_renderer_config_t debugging_config = {
    .map    = RENDER_ALWAYS,
    .stair  = RENDER_ALWAYS,
    .player = RENDER_ALWAYS,
    .enemy  = RENDER_ALWAYS,
    .item   = RENDER_ALWAYS,
};

int main() {
    srand(time(NULL));
    rand();
    int seed = rand();

    LogueCore Core = LogueCore(seed);
    logue_data_t logue_data;

    LogueGenerator Mapgen = LogueGenerator();
    logue_map_t logue_map;
    
    LogueRenderer Renderer = LogueRenderer();
    logue_renderer_config_t render_config = insight_config;

    Core.initialize();
    Mapgen.generate_map(&logue_map, Core.get_seed(), 0); //test. difficulty is 0
    Core.set_map(&logue_map);

    Core.process_turn(CMD_NONE_NO_TURN);
    Core.get_data(&logue_data);
    Core.get_map(&logue_map);
    Renderer.render_map(&logue_data, &logue_map, &render_config);

    enum logue_turn_result result;
    char input;

    while (1) {
        scanf(" %c", &input);

        switch (input) {
        case 'w':
            result = Core.process_turn(CMD_MOVE_UP);
            break;
        case 's':
            result = Core.process_turn(CMD_MOVE_DOWN);
            break;
        case 'a':
            result = Core.process_turn(CMD_MOVE_LEFT);
            break;
        case 'd':
            result = Core.process_turn(CMD_MOVE_RIGHT);
            break;
        case 'h':
            result = Core.process_turn(CMD_ATTACK);
            break;
        case 'g':
            result = Core.process_turn(CMD_SEARCH);
            break;
        default:
            break;
        }

        switch (result)
        {
        case TURN_NONE:
        case TURN_PROCESSED:
            Core.get_data(&logue_data);
            Core.get_map(&logue_map);
            Renderer.render_map(&logue_data, &logue_map, &render_config);
            break;
        case TURN_NEXT_FLOOR:
            Mapgen.generate_map(&logue_map, rand(), 0); //test. difficulty is 0
            Core.set_map(&logue_map);
            Core.process_turn(CMD_NONE_NO_TURN);

            Core.get_data(&logue_data);
            Core.get_map(&logue_map);
            Renderer.render_map(&logue_data, &logue_map, &render_config);
            break;
        case TURN_GAME_OVER:
            printf("game over...\n");
            return 0;
        default:
            break;
        }


    }

    return 0;
}