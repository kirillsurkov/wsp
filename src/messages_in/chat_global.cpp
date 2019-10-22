#include "precompiled.hpp"
#include "messages_in/chat_global.hpp"

std::shared_ptr<message::in::chat_global_t> message::in::chat_global_t::create(const rapidjson::Value& json) {
    const auto& text_doc = json["text"];
    if (!text_doc.IsString()) {
        throw std::runtime_error("Invalid global chat message text");
    }

    return std::make_shared<chat_global_t>(text_doc.GetString());
}

std::shared_ptr<message::in::chat_global_t> message::in::chat_global_t::create(binary_data_t& data) {
    return std::make_shared<chat_global_t>(data.get_string());
}

message::in::chat_global_t::chat_global_t(const std::string& text) :
    message_t(type::chat_global),
    m_text(text)
{
}

message::in::chat_global_t::~chat_global_t() {
}

const std::string& message::in::chat_global_t::get_text() const {
    return m_text;
}
