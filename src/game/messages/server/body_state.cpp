#include "precompiled.hpp"
#include "game/messages/server/body_state.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

message::out::body_state_t::body_state_t(int frame, const std::unordered_map<int, engine::physics::object_t::body_state_t>& state) :
    engine::network::message::server_t(static_cast<int>(type::body_state), frame),
    m_state(state)
{
}

message::out::body_state_t::~body_state_t() {
}

void message::out::body_state_t::write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
    writer.String("objects");
    writer.StartArray();
    for (const auto& pair : m_state) {
        auto id = pair.first;
        const auto& state = pair.second;
        writer.StartObject();
        writer.String("id");
        writer.Int(id);
        writer.String("sleeping");
        writer.Bool(state.sleeping);
        writer.String("position");
        writer.StartArray();
        writer.Double(state.position.x);
        writer.Double(state.position.y);
        writer.Double(state.position.z);
        writer.EndArray();
        writer.String("rotation");
        writer.StartArray();
        writer.Double(state.rotation.x);
        writer.Double(state.rotation.y);
        writer.Double(state.rotation.z);
        writer.Double(state.rotation.w);
        writer.EndArray();
        writer.String("linear_speed");
        writer.StartArray();
        writer.Double(state.linear_speed.x);
        writer.Double(state.linear_speed.y);
        writer.Double(state.linear_speed.z);
        writer.EndArray();
        writer.String("angular_speed");
        writer.StartArray();
        writer.Double(state.angular_speed.x);
        writer.Double(state.angular_speed.y);
        writer.Double(state.angular_speed.z);
        writer.EndArray();
        writer.EndObject();
    }
    writer.EndArray();
}

void message::out::body_state_t::write_data(binary_writer_t& writer) const {
    writer.write_uint32(static_cast<unsigned int>(m_state.size()));
    for (const auto& pair : m_state) {
        auto id = pair.first;
        const auto& state = pair.second;
        writer.write_uint16(static_cast<unsigned short>(id));
        writer.write_uint8(state.sleeping);
        writer.write_float32(state.position.x);
        writer.write_float32(state.position.y);
        writer.write_float32(state.position.z);
        writer.write_float32(state.rotation.x);
        writer.write_float32(state.rotation.y);
        writer.write_float32(state.rotation.z);
        writer.write_float32(state.rotation.w);
        writer.write_float32(state.linear_speed.x);
        writer.write_float32(state.linear_speed.y);
        writer.write_float32(state.linear_speed.z);
        writer.write_float32(state.angular_speed.x);
        writer.write_float32(state.angular_speed.y);
        writer.write_float32(state.angular_speed.z);
    }
}
