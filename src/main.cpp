#include "main.hpp"

int main() {
    srand(time(NULL));
    rand();
    int seed = rand();

    LogueCore Core = LogueCore(seed);
    LogueCoreMap logueMap;

    Core.generate_map(&logueMap);

    LogueRenderer Renderer = LogueRenderer();
    Renderer.render_map(&logueMap);

    return 0;
}