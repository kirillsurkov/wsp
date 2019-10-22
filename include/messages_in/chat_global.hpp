#pragma once

#include "message.hpp"

namespace message::in {
    class chat_global_t : public message_t {
    private:
        std::string m_text;

    public:
        static std::shared_ptr<chat_global_t> create(const rapidjson::Value& json);
        static std::shared_ptr<chat_global_t> create(binary_data_t& data);

        chat_global_t(const std::string& text);
        virtual ~chat_global_t() override;

        const std::string& get_text() const;
    };
}
