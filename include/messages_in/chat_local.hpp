#pragma once

#include "message.hpp"

namespace message::in {
    class chat_local_t : public message_t {
    private:
        std::string m_text;

    public:
        static std::shared_ptr<chat_local_t> create(const rapidjson::Value& json);
        static std::shared_ptr<chat_local_t> create(binary_reader_t& reader);

        chat_local_t(const std::string& text);
        virtual ~chat_local_t() override;

        const std::string& get_text() const;
    };
}
