#include "precompiled.hpp"
#include "messages_in/chat_local.hpp"

std::shared_ptr<message::in::chat_local_t> message::in::chat_local_t::create(const rapidjson::Value& json) {
    const auto& text_doc = json["text"];
    if (!text_doc.IsString()) {
        throw std::runtime_error("Invalid local chat message text");
    }

    return std::make_shared<chat_local_t>(text_doc.GetString());
}

std::shared_ptr<message::in::chat_local_t> message::in::chat_local_t::create(binary_reader_t& reader) {
    return std::make_shared<chat_local_t>(reader.read_string());
}

message::in::chat_local_t::chat_local_t(const std::string& text) :
    message_t(type::chat_local),
    m_text(text)
{
}

message::in::chat_local_t::~chat_local_t() {
}

const std::string& message::in::chat_local_t::get_text() const {
    return m_text;
}
