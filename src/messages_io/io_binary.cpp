#include "precompiled.hpp"
#include "messages_io/io_binary.hpp"
#include "binary_data.hpp"

io_binary_t::io_binary_t() {
}

io_binary_t::~io_binary_t() {
}

void io_binary_t::read(const unsigned char* in, unsigned int in_size, std::vector<std::shared_ptr<message::in::message_t>>& out) const {
    out.clear();
    binary_data_t data(in, in_size);
    int count = data.get_uint8();
    for (int i = 0; i < count; i++) {
        try {
            auto type = static_cast<message::in::type>(data.get_uint8());
            std::shared_ptr<message::in::message_t> message;
            CREATE_MESSAGE(type, data, message)
            out.push_back(message);
        } catch (const std::exception& e) {
            LOG << e.what();
        }
    }
}

unsigned int io_binary_t::write(const std::deque<std::shared_ptr<message::out::message_t>>& in, std::vector<unsigned char>& out) const {
    auto count = std::min(in.size(), 200ul);
    return count;
}
