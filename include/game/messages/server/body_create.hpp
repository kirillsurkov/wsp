#pragma once

#include "engine/network/message/server.hpp"
#include "engine/physics/physics_object.hpp"

#include "game/messages/types.hpp"

namespace message::out {
class body_create_t : public engine::network::message::server_t {
private:
    engine::physics::object_t::body_t m_body;

protected:
    virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;
    virtual void write_data(binary_writer_t& writer) const override;

public:
    body_create_t(int frame, const engine::physics::object_t::body_t& body);
    virtual ~body_create_t() override;
};
}
