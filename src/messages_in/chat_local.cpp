#include "precompiled.hpp"
#include "messages_in/chat_local.hpp"

message::in::chat_local_t::chat_local_t(const rapidjson::Value& json) :
    message_t(type::chat_local)
{
    const auto& text_doc = json["text"];
    if (!text_doc.IsString()) {
        throw std::runtime_error("Invalid local chat message text");
    }
    m_text = text_doc.GetString();
}

message::in::chat_local_t::~chat_local_t() {
}

const std::string& message::in::chat_local_t::get_text() const {
    return m_text;
}
