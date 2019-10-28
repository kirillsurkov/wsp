#include "precompiled.hpp"
#include "game/messages/client/login.hpp"

message::in::login_t::builder_t::~builder_t() {
}

std::shared_ptr<message::in::login_t> message::in::login_t::builder_t::build(const rapidjson::Value& json) const {
    const auto& name_doc = json["name"];
    if (!name_doc.IsString()) {
        throw std::runtime_error("Invalid login name");
    }
    return std::make_shared<message::in::login_t>(name_doc.GetString());
}

std::shared_ptr<message::in::login_t> message::in::login_t::builder_t::build(binary_reader_t& reader) const {
    return std::make_shared<message::in::login_t>(reader.read_string());
}

message::in::login_t::login_t(const std::string& name) :
    m_name(name)
{
}

message::in::login_t::~login_t() {
}

const std::string& message::in::login_t::get_name() const {
    return m_name;
}
