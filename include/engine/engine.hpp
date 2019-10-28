#pragma once

#include "game.hpp"
#include "network/network.hpp"

#include "utils/log.hpp"

namespace engine {
template <class T_game>
class engine_t {
private:
    std::shared_ptr<T_game> m_game;
    std::shared_ptr<engine::physics_t> m_physics;
    std::thread m_main_loop;

    void main_loop() {
        auto last_time = std::chrono::system_clock::now();
        while (true) {
            auto now = std::chrono::system_clock::now();
            float delta = std::chrono::duration_cast<std::chrono::microseconds>(now - last_time).count() / 1000000.0f;
            last_time = now;

            m_game->update(delta);
        }
    }

public:
    engine_t(std::shared_ptr<T_game> game) :
        m_game(game),
        m_physics(game->get_physics()),
        m_main_loop([&]() {
            main_loop();
        })
    {
        m_game->create();
        m_game->get_messages_receiver()->create();

        LOG << "Engine started";
    }

    ~engine_t() {
        LOG << "Engine stopped";
    }

    void start_server(unsigned short port, unsigned int threads) {
        network::network_t<typename T_game::messages_receiver_t, typename T_game::protocol_t, typename T_game::session_t> network(m_game->get_messages_receiver(), threads);
        network.listen(port);
        network.wait();
    }
};
}
