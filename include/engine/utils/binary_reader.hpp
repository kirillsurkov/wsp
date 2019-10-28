#pragma once

#include <string>

class binary_reader_t {
private:
    const unsigned char* m_buffer;
    unsigned long m_size;
    unsigned long m_offset;

    void check_available(unsigned int bytes);

public:
    binary_reader_t(const unsigned char* buffer, unsigned long size);
    ~binary_reader_t();

    unsigned char read_uint8();
    unsigned short read_uint16();
    unsigned int read_uint32();

    char read_int8();
    short read_int16();
    int read_int32();

    float read_float32();
    double read_float64();

    std::string read_string();

    const unsigned char* get_buffer() const;
    unsigned long get_size() const;
};
