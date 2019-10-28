#pragma once

#include <unordered_map>

#include "engine/network/message/server.hpp"
#include "engine/physics/physics_object.hpp"

#include "game/messages/types.hpp"

namespace message::out {
class body_state_t : public engine::network::message::server_t {
private:
    std::unordered_map<int, engine::physics::object_t::body_state_t> m_state;

protected:
    virtual void write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;
    virtual void write_data(binary_writer_t& writer) const override;

public:
    body_state_t(int frame, const std::unordered_map<int, engine::physics::object_t::body_state_t>& state);
    virtual ~body_state_t() override;
};
}
