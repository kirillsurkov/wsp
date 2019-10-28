#pragma once

#include "engine/network/protocol.hpp"

namespace engine::network::protocol {
template <class T_messages_receiver>
class binary_t : public protocol_t<T_messages_receiver> {
public:
    binary_t(std::shared_ptr<T_messages_receiver>& messages_receiver) :
        protocol_t<T_messages_receiver>(messages_receiver)
    {
    }

    virtual ~binary_t() override {
    }

    virtual void read(binary_reader_t& in, std::shared_ptr<engine::network::session_t>& session, std::shared_ptr<T_messages_receiver>& messages_receiver) const override {
        int count = in.read_uint8();
        for (int i = 0; i < count; i++) {
            try {
                messages_receiver->process_message(in.read_uint8(), in, session, messages_receiver);
            } catch (const std::exception& e) {
                LOG << e.what();
            }
        }
    }

    virtual unsigned int write(const std::deque<std::shared_ptr<message::server_t>>& in, binary_writer_t& out) const override {
        auto count = static_cast<unsigned int>(std::min(in.size(), 200ul));
        out.write_uint8(static_cast<unsigned char>(count));
        for (const auto& message : in) {
            message->write(out);
        }
        return count;
    }
};
}
