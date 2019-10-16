#pragma once

#include <string>
#include <rapidjson/writer.h>

namespace message::out {
    enum class type {
        login = 1,
        chat_local,
        chat_global,
        new_physics_object,
        physics_state
    };

    class message_t {
    private:
        type m_type;

    protected:
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

    public:
        message_t(message::out::type type);
        virtual ~message_t();

        void write_message(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
        std::string to_string() const;
    };
}
