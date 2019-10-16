#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include "messages_in.hpp"
#include "messages_out.hpp"

class core_t;
class session_t : public std::enable_shared_from_this<session_t> {
private:
	boost::asio::io_context& m_io;
	boost::asio::io_context::strand m_strand;

    core_t& m_core;

    boost::beast::websocket::stream<boost::beast::tcp_stream> m_ws;

    boost::beast::flat_buffer m_buffer;

    std::string m_write_buffer;

    int m_player;

    std::atomic<bool> m_connected;

    std::deque<std::shared_ptr<message::out::message_t>> m_messages_queue;

    message::in::type get_message_type(const rapidjson::Value& json) const;
    void process_message(const rapidjson::Value& json);

    void do_read();
    void do_write();

public:
    session_t(boost::asio::io_context& io, core_t& core, boost::asio::ip::tcp::socket&& socket);
    ~session_t();

    void send_message(std::shared_ptr<message::out::message_t> message);

    int get_player() const;
    void set_player(int id);

    void run();
};
