#include "precompiled.hpp"
#include "messages_in/login.hpp"

std::shared_ptr<message::in::login_t> message::in::login_t::create(const rapidjson::Value& json) {
    const auto& name_doc = json["name"];
    if (!name_doc.IsString()) {
        throw std::runtime_error("Invalid login name");
    }
    return std::make_shared<message::in::login_t>(name_doc.GetString());
}

std::shared_ptr<message::in::login_t> message::in::login_t::create(binary_reader_t& reader) {
    return std::make_shared<message::in::login_t>(reader.read_string());
}

message::in::login_t::login_t(const std::string& name) :
    message_t(type::login),
    m_name(name)
{
}

message::in::login_t::~login_t() {
}

const std::string& message::in::login_t::get_name() const {
    return m_name;
}
