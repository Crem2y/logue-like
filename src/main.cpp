#include "main.hpp"

const logue_renderer_config_t normal_config = {
    .map    = RENDER_DISCOVERED,
    .stair  = RENDER_DISCOVERED,
    .player = RENDER_SEARCHED,
    .enemy  = RENDER_SEARCHED,
    .item   = RENDER_SEARCHED,
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

    LogueGenerator Mapgen = LogueGenerator();
    logue_map_t logue_map;
    
    LogueRenderer Renderer = LogueRenderer();
    logue_renderer_config_t render_config = normal_config;

    Core.initialize();
    Mapgen.generate_map(&logue_map, Core.get_seed(), 0); //test
    Core.set_map(&logue_map);

    Core.process_turn(CMD_NONE_NO_TURN);
    Core.get_map(&logue_map);
    Renderer.render_map(&logue_map, &render_config);

    char input;

    while (1) {
        scanf(" %c", &input);

        switch (input) {
        case 'w':
            Core.process_turn(CMD_MOVE_UP);
            break;
        case 's':
            Core.process_turn(CMD_MOVE_DOWN);
            break;
        case 'a':
            Core.process_turn(CMD_MOVE_LEFT);
            break;
        case 'd':
            Core.process_turn(CMD_MOVE_RIGHT);
            break;
        case 'h':
            Core.process_turn(CMD_ATTACK);
            break;
        case 'g':
            Core.process_turn(CMD_SEARCH);
            break;
        default:
            break;
        }

        Core.get_map(&logue_map);
        Renderer.render_map(&logue_map, &render_config);
    }

    return 0;
}