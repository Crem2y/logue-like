#include "main.hpp"

int main() {
    srand(time(NULL));
    rand();
    int seed = rand();

    LogueCore Core = LogueCore(seed);
    logue_map_t logue_map;

    Core.initialize();
    Core.generate_map(&logue_map, Core.get_seed(), 0); //test

    LogueRenderer Renderer = LogueRenderer();
    logue_renderer_config_t render_config;
    Renderer.render_map(&logue_map, &render_config);

    return 0;
}