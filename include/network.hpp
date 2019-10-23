#pragma once

#include <vector>
#include <thread>

#include <boost/asio/io_context.hpp>

#include "messages_io.hpp"
#include "messages_receiver.hpp"

class network_t {
private:
    int m_threads;
    boost::asio::io_context m_io;
    std::vector<std::thread> m_threads_list;

public:
    network_t(std::shared_ptr<messages_receiver_t> messages_receiver, int port, int threads, const messages_io_t& messages_io);
    ~network_t();

    void run();
    void wait();
};
