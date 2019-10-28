#pragma once

#include <deque>

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket/stream.hpp>

#include "session.hpp"

#include "engine/utils/log.hpp"
#include "engine/utils/binary_reader.hpp"

namespace engine::network::session {
template <class T_messages_receiver, class T_protocol>
class beast_t : public session_t {
private:
    boost::asio::io_context& m_io;
    boost::asio::io_context::strand m_strand;

    boost::beast::websocket::stream<boost::beast::tcp_stream> m_ws;

    std::shared_ptr<T_messages_receiver> m_messages_receiver;
    const T_protocol& m_protocol;

    std::shared_ptr<binary_writer_t> m_write_buffer;

    std::atomic<bool> m_connected;

    std::deque<std::shared_ptr<message::server_t>> m_messages_queue;

    boost::beast::flat_buffer m_read_buffer;

    void do_read() {
        auto this_ptr = shared_from_this();
        m_ws.async_read(m_read_buffer, [this, this_ptr](boost::beast::error_code err, std::size_t size) mutable {
            if (err) {
                LOG << "session_t::do_read error: " << err.message();
                m_connected = false;
                m_messages_receiver->on_disconnect(this_ptr);
                return;
            }
            try {
                binary_reader_t reader(reinterpret_cast<unsigned char*>(m_read_buffer.data().data()), size);
                m_protocol.read(reader, this_ptr, m_messages_receiver);
            } catch (const std::exception& e) {
                LOG << e.what();
            }

            m_read_buffer.consume(size);
            do_read();
        });
    }

    void do_write() {
        if (!m_connected) {
            return;
        }
        m_write_buffer = std::make_shared<binary_writer_t>();
        auto count = m_protocol.write(m_messages_queue, *m_write_buffer);
        m_ws.async_write(boost::asio::buffer(m_write_buffer->get_buffer()), m_strand.wrap([this, count](boost::beast::error_code err, std::size_t) {
            if (err) {
                LOG << "session_t::do_write error: " << err.message();
            } else {
                m_messages_queue.erase(m_messages_queue.begin(), m_messages_queue.begin() + count);
            }
            if (!m_messages_queue.empty()) {
                do_write();
            }
        }));
    }

public:
    beast_t(boost::asio::io_context& io, boost::asio::ip::tcp::socket&& socket, std::shared_ptr<T_messages_receiver>& messages_receiver, const T_protocol& protocol) :
        session_t(),
        m_io(io),
        m_strand(m_io),
        m_ws(std::move(socket)),
        m_messages_receiver(messages_receiver),
        m_protocol(protocol),
        m_connected(false)
    {
    }

    virtual ~beast_t() override {
    }

    void run() {
        m_ws.set_option(boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server));
        m_ws.set_option(boost::beast::websocket::stream_base::decorator([](boost::beast::websocket::response_type& res) {
            res.set(boost::beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-async");
        }));
        auto this_ptr = shared_from_this();
        m_ws.async_accept([this, this_ptr](boost::beast::error_code err) {
            if (err) {
                std::cout << "Error session accept: " << err.message() << std::endl;
                return;
            }
            m_connected = true;
            do_read();
        });
    }

    virtual void send_message(const std::shared_ptr<message::server_t>& message) override {
        m_strand.post([this, message]() {
            m_messages_queue.push_back(message);
            if (m_messages_queue.size() == 1) {
                do_write();
            }
        });
    }
};
}
