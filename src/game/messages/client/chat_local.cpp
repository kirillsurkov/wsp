#include "precompiled.hpp"
#include "game/messages/client/chat_local.hpp"

message::in::chat_local_t::builder_t::~builder_t() {
}

std::shared_ptr<message::in::chat_local_t> message::in::chat_local_t::builder_t::build(const rapidjson::Value& json) const {
    const auto& text_doc = json["text"];
    if (!text_doc.IsString()) {
        throw std::runtime_error("Invalid local chat message text");
    }
    return std::make_shared<chat_local_t>(text_doc.GetString());
}

std::shared_ptr<message::in::chat_local_t> message::in::chat_local_t::builder_t::build(binary_reader_t& reader) const {
    return std::make_shared<chat_local_t>(reader.read_string());
}

message::in::chat_local_t::chat_local_t(const std::string& text) :
    m_text(text)
{
}

message::in::chat_local_t::~chat_local_t() {
}

const std::string& message::in::chat_local_t::get_text() const {
    return m_text;
}
