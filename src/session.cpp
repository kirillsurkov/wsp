#include "precompiled.hpp"
#include "session.hpp"
#include "player.hpp"
#include "core.hpp"

#define PROCESS_MESSAGE(msg) \
	case message::in::type::msg: { \
		m_core.on_message(this_ptr, message::in::msg##_t(data)); \
        break; \
    }

#define PROCESS_MESSAGES \
	PROCESS_MESSAGE(delta_state) \
	PROCESS_MESSAGE(chat_local) \
	PROCESS_MESSAGE(chat_global)

session_t::session_t(boost::asio::io_context& io, core_t& core, boost::asio::ip::tcp::socket&& socket) :
	m_io(io),
	m_strand(io),
    m_core(core),
    m_ws(std::move(socket)),
    m_player(0),
    m_connected(false)
{
}

session_t::~session_t() {
}

message::in::type session_t::get_message_type(const rapidjson::Value& json) const {
    do {
        if (!json.IsObject()) break;
        const auto& type_doc = json["type"];
        if (!type_doc.IsInt()) break;
        return static_cast<message::in::type>(type_doc.GetInt());
    } while (false);
    throw std::runtime_error("Invalid message type");
}

void session_t::process_message(const rapidjson::Value& json) {
    message::in::type type = get_message_type(json);
    const auto& data = json["data"];
    auto this_ptr = shared_from_this();
    if (!m_player && type == message::in::type::login) {
        m_core.on_message(this_ptr, message::in::login_t(data));
    } else switch (type) {
    	PROCESS_MESSAGES
        default: {
            throw std::runtime_error("Unknown message type");
        }
    }
}

void session_t::do_read() {
    auto this_ptr = shared_from_this();
    m_ws.async_read(m_buffer, [this_ptr](boost::beast::error_code err, std::size_t count) {
        if (err) {
            this_ptr->m_connected = false;
            this_ptr->m_core.on_disconnect(this_ptr);
            return;
        }
        rapidjson::Document json;
        auto jerr = json.Parse(std::string(reinterpret_cast<char*>(this_ptr->m_buffer.data().data()), count).c_str()).GetParseError();
        if (jerr == rapidjson::kParseErrorNone && json.IsArray()) {
            for (auto it = json.Begin(); it != json.End(); it++) {
                try {
                    this_ptr->process_message(*it);
                } catch (const std::exception&) {
                }
            }
        }
        this_ptr->m_buffer.consume(count);
        this_ptr->do_read();
    });
}

void session_t::do_write() {
	int count = std::min(m_messages_queue.size(), 100ul);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writer.StartArray();
	for (auto it = m_messages_queue.begin(); it != m_messages_queue.begin() + count; it++) {
		(*it)->write_message(writer);
	}
	writer.EndArray();

	m_write_buffer = std::string(buffer.GetString(), buffer.GetSize());

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
