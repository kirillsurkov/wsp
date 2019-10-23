#include "precompiled.hpp"
#include "messages_out/message.hpp"

message::out::message_t::message_t(message::out::type type, int frame) :
    m_type(type),
    m_frame(frame)
{
}

message::out::message_t::~message_t() {
}

void message::out::message_t::write_data(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
}

void message::out::message_t::write(rapidjson::Writer<rapidjson::StringBuffer>& writer) const {
    writer.StartObject();
    writer.String("type");
    writer.Int(static_cast<int>(m_type));
    writer.String("data");
    writer.StartObject();
    writer.String("frame");
    writer.Int(m_frame);
    write_data(writer);
    writer.EndObject();
    writer.EndObject();
}

std::string message::out::message_t::to_string() const {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

    write(writer);

    return buffer.GetString();
}
