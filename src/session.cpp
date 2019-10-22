#include "precompiled.hpp"
#include "session.hpp"

#include <boost/preprocessor/seq/for_each.hpp>

#define MESSAGES (login)(delta_state)(chat_local)(chat_global)

#define CREATE_MESSAGE_MACRO(r, data, msg) \
    case message::in::type::msg: { \
        created_msg = message::in::BOOST_PP_CAT(msg, _t)::create(data); \
        break; \
    }

#define PROCESS_MESSAGE_MACRO(r, data, msg) \
    case message::in::type::msg: { \
        m_messages_receiver->on_message(this_ptr, std::dynamic_pointer_cast<message::in::BOOST_PP_CAT(msg, _t)>(data)); \
        break; \
    }

#define EXPAND_MESSAGES(macro, type, data) \
    switch (type) { \
        BOOST_PP_SEQ_FOR_EACH(macro, data, MESSAGES) \
        default: { \
            throw std::runtime_error("Unknown message type"); \
        } \
    }

#define PROCESS_MESSAGE(msg) EXPAND_MESSAGES(PROCESS_MESSAGE_MACRO, msg->get_type(), msg)
#define CREATE_MESSAGE(type, data) EXPAND_MESSAGES(CREATE_MESSAGE_MACRO, type, data)

session_t::session_t(boost::asio::io_context& io, std::shared_ptr<messages_receiver_t> messages_receiver, boost::asio::ip::tcp::socket&& socket, network_t::protocol protocol) :
    m_io(io),
    m_strand(io),
    m_messages_receiver(messages_receiver),
    m_ws(std::move(socket)),
    m_protocol(protocol),
    m_player(0),
    m_connected(false)
{
}

session_t::~session_t() {
}

void session_t::process_message(const std::shared_ptr<message::in::message_t>& message) {
    auto this_ptr = shared_from_this();
    if (message->get_type() == message::in::type::login) {
        if (!m_player) {
            m_messages_receiver->on_message(this_ptr, std::dynamic_pointer_cast<message::in::login_t>(message));
        }
    } else PROCESS_MESSAGE(message)
}

void session_t::do_read() {
    auto this_ptr = shared_from_this();
    m_ws.async_read(m_buffer, [this_ptr](boost::beast::error_code err, std::size_t count) {
        if (err) {
            this_ptr->m_connected = false;
            this_ptr->m_messages_receiver->on_disconnect(this_ptr);
            return;
        }
        auto raw_bytes = static_cast<unsigned char*>(this_ptr->m_buffer.data().data());
        try {
            switch (this_ptr->m_protocol) {
                case network_t::protocol::json: {
                    rapidjson::Document json;
                    if (json.Parse(std::string(reinterpret_cast<char*>(raw_bytes), count).c_str()).GetParseError() == rapidjson::kParseErrorNone) {
                        if (!json.IsArray()) {
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
                                std::shared_ptr<message::in::message_t> created_msg;
                                CREATE_MESSAGE(type, data)
                                this_ptr->process_message(created_msg);
                            } catch (const std::exception& e) {
                                LOG << e.what();
                            }
                        }
                    }
                    break;
                }
                case network_t::protocol::binary: {
                    binary_data_t data(raw_bytes, count);
                    int count = data.get_uint8();
                    for (int i = 0; i < count; i++) {
                        try {
                            auto type = static_cast<message::in::type>(data.get_uint8());
                            std::shared_ptr<message::in::message_t> created_msg;
                            CREATE_MESSAGE(type, data)
                            this_ptr->process_message(created_msg);
                        } catch (const std::exception& e) {
                            LOG << e.what();
                        }
                    }
                    break;
                }
            }
        } catch (const std::exception& e) {
            LOG << e.what();
        }

        this_ptr->m_buffer.consume(count);
        this_ptr->do_read();
    });
}

void session_t::do_write() {
    auto count = std::min(m_messages_queue.size(), 100ul);

    switch (m_protocol) {
        case network_t::protocol::json: {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            writer.StartArray();
            for (auto it = m_messages_queue.begin(); it != m_messages_queue.begin() + count; it++) {
                (*it)->write_message(writer);
            }
            writer.EndArray();

            m_write_buffer = std::string(buffer.GetString(), buffer.GetSize());
            break;
        }
        case network_t::protocol::binary: {
            break;
        }
    }

    auto this_ptr = shared_from_this();
    m_ws.async_write(boost::asio::buffer(m_write_buffer), m_strand.wrap([this_ptr, count](boost::beast::error_code err, std::size_t) {
        if (err) {
            std::cout << "Error " << err.message() << std::endl;
        } else {
            this_ptr->m_messages_queue.erase(this_ptr->m_messages_queue.begin(), this_ptr->m_messages_queue.begin() + count);
        }
        if (!this_ptr->m_messages_queue.empty()) {
            this_ptr->do_write();
        }
    }));
}

void session_t::send_message(std::shared_ptr<message::out::message_t> message) {
    auto this_ptr = shared_from_this();
    m_strand.post([this_ptr, message]() {
        this_ptr->m_messages_queue.push_back(message);
        if (this_ptr->m_messages_queue.size() == 1) {
            this_ptr->do_write();
        }
    });
}

int session_t::get_player() const {
    return m_player;
}

void session_t::set_player(int id) {
    m_player = id;
}

void session_t::run() {
    auto this_ptr = shared_from_this();
    m_ws.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
    m_ws.set_option(boost::beast::websocket::stream_base::decorator([](boost::beast::websocket::response_type& res) {
        res.set(boost::beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-async");
    }));
    m_ws.async_accept([this_ptr](boost::beast::error_code err) {
        if (err) {
            std::cout << "Error session accept: " << err.message() << std::endl;
            return;
        }
        this_ptr->m_connected = true;
        this_ptr->do_read();
    });
}
