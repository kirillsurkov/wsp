#pragma once

#include "engine/game.hpp"
#include "engine/network/protocols/binary.hpp"
#include "messages/client/receiver.hpp"

class game_wsp_t : public message::in::receiver_t,
                   public engine::game_t<message::in::receiver_t, engine::network::protocol::binary_t>,
                   public std::enable_shared_from_this<game_wsp_t>
{
public:
    game_wsp_t();
    virtual ~game_wsp_t() override;

    virtual void create() override;

    virtual void on_disconnect(std::shared_ptr<engine::network::session_t>& session) override;
    virtual void on_message(std::shared_ptr<engine::network::session_t>& session, const message::in::login_t& message) override;
    virtual void on_message(std::shared_ptr<engine::network::session_t>& session, const message::in::delta_state_t& message) override;
    virtual void on_message(std::shared_ptr<engine::network::session_t>& session, const message::in::chat_local_t& message) override;
    virtual void on_message(std::shared_ptr<engine::network::session_t>& session, const message::in::chat_global_t& message) override;
};
