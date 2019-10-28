#pragma once

#include "engine/network/message/client.hpp"

#include "game/messages/types.hpp"

namespace message::in {
class login_t : public engine::network::message::client_t<type::login, login_t> {
public:
    class builder_t : public login_t::type::builder_t {
    public:
        virtual ~builder_t() override;
        virtual std::shared_ptr<login_t> build(const rapidjson::Value& json) const override;
        virtual std::shared_ptr<login_t> build(binary_reader_t& reader) const override;
    };

private:
    std::string m_name;

public:
    login_t(const std::string& name);
    virtual ~login_t() override;

    const std::string& get_name() const;
};
}
