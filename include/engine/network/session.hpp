#pragma once

#include <deque>

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include "message/server.hpp"

#include "engine/utils/log.hpp"

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
