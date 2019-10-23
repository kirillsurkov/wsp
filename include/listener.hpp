#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include "messages_receiver.hpp"
#include "messages_io.hpp"

class listener_t : public std::enable_shared_from_this<listener_t> {
private:
    boost::asio::io_context& m_io;
    std::shared_ptr<messages_receiver_t> m_messages_receiver;
    const messages_io_t& m_messages_io;
    boost::asio::ip::tcp::acceptor m_acceptor;

    void do_accept();

public:
    listener_t(boost::asio::io_context& io, std::shared_ptr<messages_receiver_t> messages_receiver, const boost::asio::ip::tcp::endpoint& endpoint, const messages_io_t& messages_io);
    ~listener_t();

    void run();
};
