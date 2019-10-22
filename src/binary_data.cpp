#include "precompiled.hpp"
#include "binary_data.hpp"

binary_data_t::binary_data_t(const unsigned char* data, unsigned long size) :
    m_data(data),
    m_size(size),
    m_offset(0)
{
}

binary_data_t::~binary_data_t() {
}

void binary_data_t::check_available(unsigned int bytes) {
    if (m_offset + bytes > m_size) {
        throw std::runtime_error("Binary data out of bounds");
    }
}

unsigned char binary_data_t::get_uint8() {
    check_available(1);
    auto res = *(m_data + m_offset);
    m_offset += 1;
    return res;
}

char binary_data_t::get_int8() {
    return static_cast<char>(get_uint8());
}

unsigned short binary_data_t::get_uint16() {
    check_available(2);
    auto res = *reinterpret_cast<const unsigned short*>(m_data + m_offset);
    m_offset += 2;
    return res;
}

short binary_data_t::get_int16() {
    return static_cast<short>(get_uint16());
}

unsigned int binary_data_t::get_uint32() {
    check_available(4);
    auto res = *reinterpret_cast<const unsigned int*>(m_data + m_offset);
    m_offset += 4;
    return res;
}

int binary_data_t::get_int32() {
    return static_cast<int>(get_uint32());
}

float binary_data_t::get_float32() {
    check_available(4);
    auto res = *reinterpret_cast<const float*>(m_data + m_offset);
    m_offset += 4;
    return res;
}

double binary_data_t::get_float64() {
    check_available(8);
    auto res = *reinterpret_cast<const double*>(m_data + m_offset);
    m_offset += 8;
    return res;
}

std::string binary_data_t::get_string() {
    auto size = get_uint8();
    check_available(size);
    std::string res(reinterpret_cast<const char*>(m_data + m_offset), size);
    m_offset += size;
    return res;
}
