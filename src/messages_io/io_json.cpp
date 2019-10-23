#include "precompiled.hpp"
#include "messages_io/io_json.hpp"

io_json_t::io_json_t() {
}

io_json_t::~io_json_t() {
}

void io_json_t::read(const unsigned char* in, unsigned int in_size, std::vector<std::shared_ptr<message::in::message_t>>& out) const {
    out.clear();
    rapidjson::Document json;
    if (json.Parse(std::string(reinterpret_cast<const char*>(in), in_size).c_str()).GetParseError() != rapidjson::kParseErrorNone || !json.IsArray()) {
        throw std::runtime_error("Bad json");
    }
    for (auto it = json.Begin(); it != json.End(); it++) {
        try {
            const auto& json = *it;
            if (!json.IsObject() || !json["type"].IsInt()) {
                throw std::runtime_error("Bad message format");
            }
            auto type = static_cast<message::in::type>(json["type"].GetInt());
            const auto& data = json["data"];
            std::shared_ptr<message::in::message_t> message;
            CREATE_MESSAGE(type, data, message)
            out.push_back(message);
        } catch (const std::exception& e) {
            LOG << e.what();
        }
    }
}

unsigned int io_json_t::write(const std::deque<std::shared_ptr<message::out::message_t>>& in, std::vector<unsigned char>& out) const {
    auto count = std::min(in.size(), 100ul);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    writer.StartArray();
    for (auto it = in.begin(); it != in.begin() + count; it++) {
        (*it)->write(writer);
    }
    writer.EndArray();

    const auto* begin = reinterpret_cast<const unsigned char*>(buffer.GetString());
    const auto* end = begin + buffer.GetSize();
    out = std::vector<unsigned char>(begin, end);

    return count;
}
