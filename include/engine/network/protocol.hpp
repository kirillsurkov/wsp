#pragma once

#include <vector>
#include <deque>
#include <variant>

#include "engine/network/session.hpp"

#include "engine/network/message/client.hpp"
#include "engine/network/message/server.hpp"

#include "engine/utils/log.hpp"

namespace engine::network {
template <class T_messages_receiver>
class protocol_t {
private:
    std::shared_ptr<T_messages_receiver> m_messages_receiver;

public:
    protocol_t(std::shared_ptr<T_messages_receiver>& messages_receiver) :
        m_messages_receiver(messages_receiver)
    {
    }

    virtual ~protocol_t() {
    }

    virtual void read(binary_reader_t& in, std::shared_ptr<session_t>& session, std::shared_ptr<T_messages_receiver>& messages_receiver) const = 0;
    virtual unsigned int write(const std::deque<std::shared_ptr<message::server_t>>& in, binary_writer_t& out) const = 0;
};
}
