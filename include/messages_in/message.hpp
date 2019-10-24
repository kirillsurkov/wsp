#pragma once

#include <memory>
#include <rapidjson/document.h>

#include "binary_reader.hpp"

#define MESSAGES_IN (login)(delta_state)(chat_local)(chat_global)

namespace message::in {
    enum class type {
        login = 1,
        delta_state,
        chat_local,
        chat_global
    };

    class message_t {
    private:
        type m_type;

    public:
        message_t(message::in::type type);
        virtual ~message_t();

        type get_type() const;
    };
}
