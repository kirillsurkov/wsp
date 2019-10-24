#include "precompiled.hpp"
#include "binary_writer.hpp"

binary_writer_t::binary_writer_t() {
}

binary_writer_t::~binary_writer_t() {
}

void binary_writer_t::write_uint8(unsigned char value) {
    m_buffer.push_back(value);
}

void binary_writer_t::write_uint16(unsigned short value) {
    auto* data = reinterpret_cast<unsigned char*>(&value);
    m_buffer.insert(m_buffer.end(), data, data + 2);
}

void binary_writer_t::write_uint32(unsigned int value) {
    unsigned char* data = reinterpret_cast<unsigned char*>(&value);
    m_buffer.insert(m_buffer.end(), data, data + 4);
}

void binary_writer_t::write_float32(float value) {
    unsigned char* data = reinterpret_cast<unsigned char*>(&value);
    m_buffer.insert(m_buffer.end(), data, data + 4);
}

void binary_writer_t::write_float64(double value) {
    unsigned char* data = reinterpret_cast<unsigned char*>(&value);
    m_buffer.insert(m_buffer.end(), data, data + 8);
}

void binary_writer_t::write_string(const std::string& value) {
    write_uint8(static_cast<unsigned char>(value.size()));
    m_buffer.insert(m_buffer.end(), value.begin(), value.end());
}

void binary_writer_t::write(const unsigned char* data, unsigned long size) {
    m_buffer.insert(m_buffer.end(), data, data + size);
}

const std::vector<unsigned char>& binary_writer_t::get_buffer() const {
    return m_buffer;
}
