#include "precompiled.hpp"
#include "messages_io/io_binary.hpp"
#include "binary_reader.hpp"

io_binary_t::io_binary_t() {
}

io_binary_t::~io_binary_t() {
}

void io_binary_t::read(binary_reader_t& in, std::vector<std::shared_ptr<message::in::message_t>>& out) const {
    out.clear();
    int count = in.read_uint8();
    for (int i = 0; i < count; i++) {
        try {
            auto type = static_cast<message::in::type>(in.read_uint8());
            std::shared_ptr<message::in::message_t> message;
            CREATE_MESSAGE(type, in, message)
            out.push_back(message);
        } catch (const std::exception& e) {
            LOG << e.what();
        }
    }
}

unsigned int io_binary_t::write(const std::deque<std::shared_ptr<message::out::message_t>>& in, binary_writer_t& out) const {
    auto count = static_cast<unsigned int>(std::min(in.size(), 200ul));
    out.write_uint8(static_cast<unsigned char>(count));
    for (const auto& message : in) {
        message->write(out);
    }
    return count;
}
