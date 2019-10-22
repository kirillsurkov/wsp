#pragma once

#include <string>

class binary_data_t {
private:
    const unsigned char* m_data;
    unsigned long m_size;
    unsigned long m_offset;

    void check_available(unsigned int bytes);

public:
    binary_data_t(const unsigned char* data, unsigned long size);
    ~binary_data_t();

    unsigned char get_uint8();
    unsigned short get_uint16();
    unsigned int get_uint32();

    char get_int8();
    short get_int16();
    int get_int32();

    float get_float32();
    double get_float64();

    std::string get_string();
};
