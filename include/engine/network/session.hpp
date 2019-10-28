#pragma once

#include <memory>

#include "message/server.hpp"

namespace engine::network {
class session_t : public std::enable_shared_from_this<session_t> {
private:
    int m_player;

public:
    session_t();
    virtual ~session_t();

    virtual void send_message(const std::shared_ptr<message::server_t>& message) = 0;

    void set_player(int player);
    int get_player() const;
};
}
