#pragma once

#include <memory>
#include <rapidjson/document.h>

#include "binary_data.hpp"

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
