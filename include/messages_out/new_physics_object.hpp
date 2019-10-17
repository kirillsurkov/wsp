#pragma once

#include "messages_out/message.hpp"
#include "physics.hpp"

namespace message::out {
    class new_physics_object_t : public message_t {
    private:
        physics_t::object_t m_object;

    protected:
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

    public:
        new_physics_object_t(int frame, const physics_t::object_t& object);
        virtual ~new_physics_object_t() override;
    };
}
