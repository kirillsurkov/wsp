#pragma once

#include "engine/network/protocol.hpp"

namespace engine::network::protocol {
template <class T_messages_receiver>
class json_t : public protocol_t<T_messages_receiver> {
public:
    json_t(std::shared_ptr<T_messages_receiver> messages_receiver) :
        protocol_t<T_messages_receiver>(messages_receiver)
    {
    }

    virtual ~json_t() override {
    }

    virtual void read(binary_reader_t& in, std::shared_ptr<engine::network::session_t>& session, std::shared_ptr<T_messages_receiver>& messages_receiver) const override {
        rapidjson::Document json;
        if (json.Parse(std::string(reinterpret_cast<const char*>(in.get_buffer()), in.get_size()).c_str()).GetParseError() != rapidjson::kParseErrorNone || !json.IsArray()) {
            throw std::runtime_error("Bad json");
        }
        for (auto it = json.Begin(); it != json.End(); it++) {
            try {
                const auto& json = *it;
                if (!json.IsObject() || !json["type"].IsInt()) {
                    throw std::runtime_error("Bad message format");
                }
                messages_receiver->process_message(json["type"].GetInt(), json["data"], session, messages_receiver);
            } catch (const std::exception& e) {
                LOG << e.what();
            }
        }
    }

    virtual unsigned int write(const std::deque<std::shared_ptr<message::server_t>>& in, binary_writer_t& out) const override {
        auto count = static_cast<unsigned int>(std::min(in.size(), 100ul));

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        writer.StartArray();
        for (auto it = in.begin(); it != in.begin() + count; it++) {
            (*it)->write(writer);
        }
        writer.EndArray();
        out.write(reinterpret_cast<const unsigned char*>(buffer.GetString()), buffer.GetSize());

        return count;
    }
};
}
