#pragma once

#include <string>
#include <rapidjson/writer.h>

namespace message::out {
    enum class type {
        login = 1,
        chat_local,
        chat_global,
        body_create,
        body_state
    };

    class message_t {
    private:
        type m_type;
        int m_frame;

    protected:
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    public:
        message_t(message::out::type type, int frame);
        virtual ~message_t();

        void write_message(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

        std::string to_string() const;
    };
}
