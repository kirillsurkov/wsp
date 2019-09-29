#pragma once

#include "message.hpp"

namespace message::in {
    class chat_local_t : public message_t {
    private:
        std::string m_text;

    public:
        chat_local_t(const rapidjson::Value& json);
        virtual ~chat_local_t();

        const std::string& get_text() const;
    };
}
