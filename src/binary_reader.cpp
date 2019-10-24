#include "precompiled.hpp"
#include "binary_reader.hpp"

binary_reader_t::binary_reader_t(const unsigned char* buffer, unsigned long size) :
    m_buffer(buffer),
    m_size(size),
    m_offset(0)
{
}

binary_reader_t::~binary_reader_t() {
}

void binary_reader_t::check_available(unsigned int bytes) {
    if (m_offset + bytes > m_size) {
        throw std::runtime_error("binary_reader_t: data out of bounds");
    }
}

unsigned char binary_reader_t::read_uint8() {
    check_available(1);
    auto res = *(m_buffer + m_offset);
    m_offset += 1;
    return res;
}

char binary_reader_t::read_int8() {
    return static_cast<char>(read_uint8());
}

unsigned short binary_reader_t::read_uint16() {
    check_available(2);
    auto res = *reinterpret_cast<const unsigned short*>(m_buffer + m_offset);
    m_offset += 2;
    return res;
}

short binary_reader_t::read_int16() {
    return static_cast<short>(read_uint16());
}

unsigned int binary_reader_t::read_uint32() {
    check_available(4);
    auto res = *reinterpret_cast<const unsigned int*>(m_buffer + m_offset);
    m_offset += 4;
    return res;
}

int binary_reader_t::read_int32() {
    return static_cast<int>(read_uint32());
}

float binary_reader_t::read_float32() {
    check_available(4);
    auto res = *reinterpret_cast<const float*>(m_buffer + m_offset);
    m_offset += 4;
    return res;
}

double binary_reader_t::read_float64() {
    check_available(8);
    auto res = *reinterpret_cast<const double*>(m_buffer + m_offset);
    m_offset += 8;
    return res;
}

std::string binary_reader_t::read_string() {
    auto size = read_uint8();
    check_available(size);
    std::string res(reinterpret_cast<const char*>(m_buffer + m_offset), size);
    m_offset += size;
    return res;
}

const unsigned char* binary_reader_t::get_buffer() const {
    return m_buffer;
}

unsigned long binary_reader_t::get_size() const {
    return m_size;
}
