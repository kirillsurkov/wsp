#pragma once

#include <string>
#include <rapidjson/writer.h>

#include "binary_writer.hpp"

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
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;
        virtual void write_data(binary_writer_t& writer) const = 0;

    public:
        message_t(message::out::type type, int frame);
        virtual ~message_t();

        void write(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;
        void write(binary_writer_t& writer) const;
    };
}
