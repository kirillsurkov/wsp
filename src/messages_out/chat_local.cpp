#include "precompiled.hpp"
#include "messages_out/chat_local.hpp"

message::out::chat_local_t::chat_local_t(int sender_id, const std::string& text) :
    message_t(type::chat_local),
    m_sender_id(sender_id),
    m_text(text)
{
}

message::out::chat_local_t::~chat_local_t() {
}

void message::out::chat_local_t::write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
    writer.String("id");
    writer.Int(m_sender_id);
    writer.String("text");
    writer.String(m_text.c_str());
}
