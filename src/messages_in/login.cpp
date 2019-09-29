#include "precompiled.hpp"
#include "messages_in/login.hpp"

message::in::login_t::login_t(const rapidjson::Value& json) :
    message_t(type::login)
{
    const auto& name_doc = json["name"];
    if (!name_doc.IsString()) {
        throw std::runtime_error("Invalid login name");
    }
    m_name = name_doc.GetString();
}

message::in::login_t::~login_t() {
}

const std::string& message::in::login_t::get_name() const {
    return m_name;
}
