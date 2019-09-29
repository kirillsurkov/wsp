#include "precompiled.hpp"
#include "messages_in/chat_global.hpp"

message::in::chat_global_t::chat_global_t(const rapidjson::Value& json) :
    message_t(type::chat_global)
{
    const auto& text_doc = json["text"];
    if (!text_doc.IsString()) {
        throw std::runtime_error("Invalid global chat message text");
    }
    m_text = text_doc.GetString();
}

message::in::chat_global_t::~chat_global_t() {
}

const std::string& message::in::chat_global_t::get_text() const {
    return m_text;
}
