#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

class core_t;
class listener_t : public std::enable_shared_from_this<listener_t> {
private:
	boost::asio::io_context& m_io;

    core_t& m_core;

    boost::asio::ip::tcp::acceptor m_acceptor;

    void do_accept();

public:
    listener_t(boost::asio::io_context& io, core_t& core, const boost::asio::ip::tcp::endpoint& endpoint);
    ~listener_t();

    void run();
};
