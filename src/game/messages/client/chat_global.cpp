#include "precompiled.hpp"
#include "game/messages/client/chat_global.hpp"

message::in::chat_global_t::builder_t::~builder_t() {
}

std::shared_ptr<message::in::chat_global_t> message::in::chat_global_t::builder_t::build(const rapidjson::Value& json) const {
    const auto& text_doc = json["text"];
    if (!text_doc.IsString()) {
        throw std::runtime_error("Invalid global chat message text");
    }
    return std::make_shared<chat_global_t>(text_doc.GetString());
}

std::shared_ptr<message::in::chat_global_t> message::in::chat_global_t::builder_t::build(binary_reader_t& reader) const {
    return std::make_shared<chat_global_t>(reader.read_string());
}

message::in::chat_global_t::chat_global_t(const std::string& text) :
    m_text(text)
{
}

message::in::chat_global_t::~chat_global_t() {
}

const std::string& message::in::chat_global_t::get_text() const {
    return m_text;
}
