#include "precompiled.hpp"
#include "messages_out/new_physics_object.hpp"

message::out::new_physics_object_t::new_physics_object_t(int frame, const physics_t::object_t& object) :
    message_t(type::new_physics_object),
    m_frame(frame),
    m_object(object)
{
}

message::out::new_physics_object_t::~new_physics_object_t() {
}

void message::out::new_physics_object_t::write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
    writer.String("frame");
    writer.Int(m_frame);
    writer.String("id");
    writer.Int(m_object.id);
    writer.String("moving");
    writer.Bool(m_object.moving);
    writer.String("position");
    writer.StartArray();
    writer.Double(m_object.position.x);
    writer.Double(m_object.position.y);
    writer.Double(m_object.position.z);
    writer.EndArray();
    writer.String("rotation");
    writer.StartArray();
    writer.Double(m_object.rotation.x);
    writer.Double(m_object.rotation.y);
    writer.Double(m_object.rotation.z);
    writer.Double(m_object.rotation.w);
    writer.EndArray();
    writer.String("rotation_factor");
    writer.StartArray();
    writer.Double(m_object.rotation_factor.x);
    writer.Double(m_object.rotation_factor.y);
    writer.Double(m_object.rotation_factor.z);
    writer.EndArray();
    writer.String("parts");
    writer.StartArray();
    for (const auto& part : m_object.parts) {
        writer.StartObject();
        writer.String("type");
        writer.Int(static_cast<int>(part->type));
        writer.String("position");
        writer.StartArray();
        writer.Double(part->position.x);
        writer.Double(part->position.y);
        writer.Double(part->position.z);
        writer.EndArray();
        writer.String("rotation");
        writer.StartArray();
        writer.Double(part->rotation.x);
        writer.Double(part->rotation.y);
        writer.Double(part->rotation.z);
        writer.Double(part->rotation.w);
        writer.EndArray();
        writer.String("size");
        writer.StartArray();
        writer.Double(part->size.x);
        writer.Double(part->size.y);
        writer.Double(part->size.z);
        writer.EndArray();
        writer.EndObject();
    }
    writer.EndArray();
}
