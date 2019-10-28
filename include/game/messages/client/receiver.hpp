#pragma once

#include "engine/network/session.hpp"
#include "engine/network/message/receiver.hpp"

#include "login.hpp"
#include "delta_state.hpp"
#include "chat_local.hpp"
#include "chat_global.hpp"

namespace message::in {
class receiver_t : public engine::network::message::receiver_t<
        message::in::login_t,
        message::in::delta_state_t,
        message::in::chat_local_t,
        message::in::chat_global_t>
{
public:
    virtual ~receiver_t() override;
};
}
