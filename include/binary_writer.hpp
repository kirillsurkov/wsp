#pragma once

#include <vector>
#include <string>

class binary_writer_t {
private:
    std::vector<unsigned char> m_buffer;

public:
    binary_writer_t();
    ~binary_writer_t();

    void write_uint8(unsigned char value);
    void write_uint16(unsigned short value);
    void write_uint32(unsigned int value);

    void write_float32(float value);
    void write_float64(double value);

    void write_string(const std::string& value);

    void write(const unsigned char* data, unsigned long size);

    const std::vector<unsigned char>& get_buffer() const;
};
