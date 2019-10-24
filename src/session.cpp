#include "precompiled.hpp"
#include "session.hpp"

session_t::session_t(boost::asio::io_context& io, std::shared_ptr<messages_receiver_t> messages_receiver, boost::asio::ip::tcp::socket&& socket, const messages_io_t& messages_io) :
    m_io(io),
    m_strand(io),
    m_messages_receiver(messages_receiver),
    m_ws(std::move(socket)),
    m_messages_io(messages_io),
    m_player(0),
    m_connected(false)
{
    m_ws.binary(true);
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
    m_ws.async_read(m_read_buffer, [this_ptr](boost::beast::error_code err, std::size_t size) {
        if (err) {
            LOG << "sesion_t::do_read error: " << err.message();
            this_ptr->m_connected = false;
            this_ptr->m_messages_receiver->on_disconnect(this_ptr);
            return;
        }
        binary_reader_t reader(reinterpret_cast<unsigned char*>(this_ptr->m_read_buffer.data().data()), size);
        std::vector<std::shared_ptr<message::in::message_t>> parsed_messages;
        try {
            this_ptr->m_messages_io.read(reader, parsed_messages);
        } catch (const std::exception& e) {
            LOG << e.what();
        }

        for (const auto& message : parsed_messages) {
            this_ptr->process_message(message);
        }

        this_ptr->m_read_buffer.consume(size);
        this_ptr->do_read();
    });
}

void session_t::do_write() {
    if (!m_connected) {
        return;
    }
    m_write_buffer = std::make_shared<binary_writer_t>();
    auto count = m_messages_io.write(m_messages_queue, *m_write_buffer);
    auto this_ptr = shared_from_this();
    m_ws.async_write(boost::asio::buffer(m_write_buffer->get_buffer()), m_strand.wrap([this_ptr, count](boost::beast::error_code err, std::size_t) {
        if (err) {
            std::cout << "session_t::do_write error: " << err.message() << std::endl;
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
