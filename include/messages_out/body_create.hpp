#pragma once

#include "messages_out/message.hpp"
#include "physics_object.hpp"

namespace message::out {
    class body_create_t : public message_t {
    private:
        const physics_object_t::body_t& m_body;

    protected:
        virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;
        virtual void write_data(binary_writer_t& writer) const override;

    public:
        body_create_t(int frame, const physics_object_t::body_t& body);
        virtual ~body_create_t() override;
    };
}
