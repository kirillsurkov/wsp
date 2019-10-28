#pragma once

#include <vector>
#include <thread>

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include "engine/utils/log.hpp"

namespace engine::network {
template <class T_messages_receiver, class T_protocol, class T_session_beast>
class network_t {
private:
    std::shared_ptr<T_messages_receiver> m_messages_receiver;
    T_protocol m_protocol;
    unsigned int m_threads;
    boost::asio::io_context m_io;
    boost::asio::ip::tcp::acceptor m_acceptor;
    std::vector<std::thread> m_threads_list;

    void do_accept() {
        m_acceptor.async_accept(boost::asio::make_strand(m_io), [&](boost::beast::error_code err, boost::asio::ip::tcp::socket socket) {
            if (err) {
                LOG << "Error listener accept: " << err.message();
            } else {
                std::make_shared<T_session_beast>(m_io, std::move(socket), m_messages_receiver, m_protocol)->run();
            }
            do_accept();
        });
    }

public:
    network_t(std::shared_ptr<T_messages_receiver>& messages_receiver, unsigned int threads) :
        m_messages_receiver(messages_receiver),
        m_protocol(m_messages_receiver),
        m_threads(threads),
        m_io(m_threads),
        m_acceptor(m_io)
    {
    }

    ~network_t() {
        wait();
        LOG << "Server stopped";
    }

    void listen(unsigned short port) {
        auto endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("0.0.0.0"), port);

        boost::beast::error_code err;
        try {
            m_acceptor.open(endpoint.protocol(), err);
            if (err) throw std::runtime_error("open");

            m_acceptor.set_option(boost::asio::socket_base::reuse_address(true), err);
            if (err) throw std::runtime_error("set_option");

            m_acceptor.bind(endpoint, err);
            if (err) throw std::runtime_error("bind");

            m_acceptor.listen(boost::asio::socket_base::max_listen_connections, err);
            if (err) throw std::runtime_error("listen");

            LOG << "Server started at port " << endpoint.port();
        } catch (const std::exception& e) {
            std::cout << "Error " << e.what();
            if (err) {
                std::cout << ": " << err.message();
            }
            std::cout << std::endl;
        }

        do_accept();

        for (int i = 0; i < m_threads; i++) {
            m_threads_list.emplace_back([this]() {
                m_io.run();
            });
        }
    }

    void wait() {
        for (auto& thread : m_threads_list) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
};
}
