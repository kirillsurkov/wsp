#include "precompiled.hpp"
#include "game/messages/server/login.hpp"

message::out::login_t::login_t(int frame) :
    engine::network::message::server_t(static_cast<int>(type::login), frame),
    m_frame(frame)
{
}

message::out::login_t::~login_t() {
}

void message::out::login_t::write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
}

void message::out::login_t::write_data(binary_writer_t& writer) const {
}
