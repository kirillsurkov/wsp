#pragma once

#include <vector>
#include <thread>

#include <boost/asio/io_context.hpp>

#include "messages_receiver.hpp"

class listener_t;
class network_t {
public:
    enum class writer_type {
        json,
        binary
    };

private:
    int m_threads;
    boost::asio::io_context m_io;
    std::shared_ptr<listener_t> m_listener;
    std::vector<std::thread> m_threads_list;

public:
    network_t(std::shared_ptr<messages_receiver_t> messages_receiver, int port, int threads, writer_type writer);
    ~network_t();

    void run();
    void wait();
};
