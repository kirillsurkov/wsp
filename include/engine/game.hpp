#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

//#include "engine/physics/physics.hpp"
#include "network/session_beast.hpp"
#include "network/message/receiver.hpp"

namespace engine {
template <class T_messages_receiver, template<class> class T_protocol>
class game_t {
public:
    typedef T_messages_receiver messages_receiver_t;
    typedef T_protocol<T_messages_receiver> protocol_t;
    typedef network::session::beast_t<messages_receiver_t, protocol_t> session_t;

private:
    std::shared_ptr<T_messages_receiver> m_messages_receiver;

protected:
    void set_messages_receiver(std::shared_ptr<T_messages_receiver> messages_receiver) {
        m_messages_receiver = messages_receiver;
    }

    /*void set_physics(std::shared_ptr<engine::physics_t> physics) {
    }*/

public:
    virtual ~game_t() {
    }

    virtual void create() = 0;

    std::shared_ptr<T_messages_receiver>& get_messages_receiver() {
        return m_messages_receiver;
    }
};
}
