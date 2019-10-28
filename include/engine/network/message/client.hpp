#pragma once

#include <memory>
#include <rapidjson/document.h>

#include "engine/utils/binary_reader.hpp"

namespace engine::network::message {
template <int V_id, class T_message>
class client_t {
public:
    typedef client_t<V_id, T_message> type;

    class builder_t {
    public:
        virtual ~builder_t() {}
        virtual std::shared_ptr<T_message> build(const rapidjson::Value& json) const = 0;
        virtual std::shared_ptr<T_message> build(binary_reader_t& reader) const = 0;
    };

public:
    inline static constexpr int id = V_id;

    client_t() {
    }

    virtual ~client_t() {
    }
};
}
