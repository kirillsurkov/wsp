#pragma once

#include "messages_io.hpp"

class io_binary_t : public messages_io_t {
public:
    io_binary_t();
    virtual ~io_binary_t() override;

    virtual void read(binary_reader_t& in, std::vector<std::shared_ptr<message::in::message_t>>& out) const override;
    virtual unsigned int write(const std::deque<std::shared_ptr<message::out::message_t>>& in, binary_writer_t& out) const override;
};