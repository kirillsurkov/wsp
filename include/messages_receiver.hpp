#pragma once

#include "messages_in.hpp"

class session_t;
class messages_receiver_t {
public:
    messages_receiver_t();
    virtual ~messages_receiver_t();

    virtual void on_disconnect(std::shared_ptr<session_t> session) = 0;
    virtual void on_message(std::shared_ptr<session_t> session, const message::in::login_t& message) = 0;
    virtual void on_message(std::shared_ptr<session_t> session, const message::in::delta_state_t& message) = 0;
    virtual void on_message(std::shared_ptr<session_t> session, const message::in::chat_local_t& message) = 0;
    virtual void on_message(std::shared_ptr<session_t> session, const message::in::chat_global_t& message) = 0;
};
