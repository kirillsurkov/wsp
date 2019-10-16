#include "precompiled.hpp"
#include "messages_out/physics_state.hpp"

message::out::physics_state_t::physics_state_t(int frame, const std::unordered_map<int, physics_t::object_state_t>& state) :
	message_t(type::physics_state),
	m_frame(frame),
	m_state(state)
{
}

message::out::physics_state_t::~physics_state_t() {
}

void message::out::physics_state_t::write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
	writer.String("frame");
	writer.Int(m_frame);
	writer.String("objects");
	writer.StartArray();
	for (auto& pair : m_state) {
		auto& id = pair.first;
		auto& object = pair.second;
		writer.StartObject();
		writer.String("id");
		writer.Int(id);
		writer.String("position");
		writer.StartArray();
		writer.Double(object.position.x);
		writer.Double(object.position.y);
		writer.Double(object.position.z);
		writer.EndArray();
		writer.String("rotation");
		writer.StartArray();
		writer.Double(object.rotation.x);
		writer.Double(object.rotation.y);
		writer.Double(object.rotation.z);
		writer.Double(object.rotation.w);
		writer.EndArray();
		writer.String("linear_speed");
		writer.StartArray();
		writer.Double(object.linear_speed.x);
		writer.Double(object.linear_speed.y);
		writer.Double(object.linear_speed.z);
		writer.EndArray();
		writer.String("angular_speed");
		writer.StartArray();
		writer.Double(object.angular_speed.x);
		writer.Double(object.angular_speed.y);
		writer.Double(object.angular_speed.z);
		writer.EndArray();
		writer.EndObject();
	}
	writer.EndArray();
}
