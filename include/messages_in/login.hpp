#pragma once

#include "message.hpp"

namespace message::in {
    class login_t : public message_t {
    private:
        std::string m_name;

    public:
        login_t(const rapidjson::Value& json);
        virtual ~login_t();

        const std::string& get_name() const;
    };
}
