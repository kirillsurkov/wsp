#include "precompiled.hpp"
#include "engine/engine.hpp"
#include "game/game_wsp.hpp"

int main() {
    const unsigned short port = 34851;
    const unsigned int threads = 4;

    auto game = std::make_shared<game_wsp_t>();
    engine::engine_t<game_wsp_t>(game).start_server(port, threads);

    return 0;
}
