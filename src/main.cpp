#include "main.hpp"

int main() {
    srand(time(NULL));
    rand();
    int seed = rand();

    LogueCore Core = LogueCore(seed);

    LogueGenerator Mapgen = LogueGenerator();
    logue_map_t logue_map;
    
    LogueRenderer Renderer = LogueRenderer();
    logue_renderer_config_t render_config;

    Core.initialize();
    Mapgen.generate_map(&logue_map, Core.get_seed(), 0); //test
    Core.set_map(&logue_map);

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
        default:
            break;
        }

        Renderer.render_map(&logue_map, &render_config);
    }

    return 0;
}