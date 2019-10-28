#pragma once

#include "game.hpp"
#include "network/network.hpp"

namespace engine {
template <class T_game>
class engine_t {
private:
    std::shared_ptr<T_game> m_game;

public:
    engine_t(std::shared_ptr<T_game> game) :
        m_game(game)
    {
        m_game->create();
        m_game->get_messages_receiver()->create();
    }

    ~engine_t() {
    }

    void start_server(unsigned short port, unsigned int threads) {
        network::network_t<typename T_game::messages_receiver_t, typename T_game::protocol_t, typename T_game::session_t> network(m_game->get_messages_receiver(), threads);
        network.listen(port);
        network.wait();
    }
};
}
