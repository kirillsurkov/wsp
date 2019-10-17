#pragma once

#include "message.hpp"

namespace message::in {
    class chat_global_t : public message_t {
    private:
        std::string m_text;

    public:
        chat_global_t(const rapidjson::Value& json);
        virtual ~chat_global_t() override;

        const std::string& get_text() const;
    };
}
