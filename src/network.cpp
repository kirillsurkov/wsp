#include "precompiled.hpp"
#include "network.hpp"
#include "listener.hpp"

network_t::network_t(std::shared_ptr<messages_receiver_t> messages_receiver, int port, int threads, network_t::protocol protocol) :
    m_threads(threads),
    m_io(m_threads)
{
    auto listener = std::make_shared<listener_t>(
        m_io,
        messages_receiver,
        boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("0.0.0.0"), port),
        protocol
    );
    listener->run();
}

network_t::~network_t() {
    wait();
}

void network_t::run() {
    for (int i = 0; i < m_threads; i++) {
        m_threads_list.emplace_back([this]() {
            m_io.run();
        });
    }
}

void network_t::wait() {
    for (auto& thread : m_threads_list) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
