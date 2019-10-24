#pragma once

#include <deque>

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include "messages_receiver.hpp"
#include "messages_io.hpp"

class session_t : public std::enable_shared_from_this<session_t> {
private:
    boost::asio::io_context& m_io;
    boost::asio::io_context::strand m_strand;

    std::shared_ptr<messages_receiver_t> m_messages_receiver;

    boost::beast::websocket::stream<boost::beast::tcp_stream> m_ws;

    const messages_io_t& m_messages_io;

    boost::beast::flat_buffer m_read_buffer;
    std::shared_ptr<binary_writer_t> m_write_buffer;

    int m_player;

    std::atomic<bool> m_connected;

    std::deque<std::shared_ptr<message::out::message_t>> m_messages_queue;

    void process_message(const std::shared_ptr<message::in::message_t>& message);

    void do_read();
    void do_write();

public:
    session_t(boost::asio::io_context& io, std::shared_ptr<messages_receiver_t> messages_receiver, boost::asio::ip::tcp::socket&& socket, const messages_io_t& messages_io);
    ~session_t();

    void send_message(std::shared_ptr<message::out::message_t> message);

    int get_player() const;
    void set_player(int id);

    void run();
};
