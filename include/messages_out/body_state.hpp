#pragma once

#include <unordered_map>

#include "messages_out/message.hpp"
#include "physics_object.hpp"

namespace message::out {
    class body_state_t : public message_t {
    private:
        std::unordered_map<int, physics_object_t::body_state_t> m_state;

    protected:
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

    public:
        body_state_t(int frame, const std::unordered_map<int, physics_object_t::body_state_t>& state);
        virtual ~body_state_t() override;
    };
}
