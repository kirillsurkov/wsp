#include "precompiled.hpp"
#include "messages_out/message.hpp"

message::out::message_t::message_t(message::out::type type, int frame) :
    m_type(type),
    m_frame(frame)
{
}

message::out::message_t::~message_t() {
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

void message::out::message_t::write(binary_writer_t& writer) const {
    writer.write_uint8(static_cast<unsigned char>(m_type));
    writer.write_uint32(m_frame);
    write_data(writer);
}
