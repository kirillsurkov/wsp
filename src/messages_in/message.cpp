#include "precompiled.hpp"
#include "messages_in/message.hpp"

message::in::message_t::message_t(message::in::type type) :
    m_type(type)
{
}

message::in::message_t::~message_t() {
}

message::in::type message::in::message_t::get_type() const {
    return m_type;
}
