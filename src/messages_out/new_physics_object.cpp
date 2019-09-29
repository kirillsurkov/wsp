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
	writer.String("counter");
	writer.Int(m_frame);
	writer.String("id");
	writer.Int(m_object.id);
	writer.String("type");
	writer.Int(static_cast<int>(m_object.type));
	writer.String("moving");
	writer.Bool(m_object.moving);
	writer.String("size");
	writer.StartArray();
	writer.Double(m_object.size.x);
	writer.Double(m_object.size.y);
	writer.Double(m_object.size.z);
	writer.EndArray();
	writer.String("position");
	writer.StartArray();
	writer.Double(m_object.position.x);
	writer.Double(m_object.position.y);
	writer.Double(m_object.position.z);
	writer.EndArray();
}
