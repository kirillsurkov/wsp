#include "precompiled.hpp"
#include "listener.hpp"
#include "session.hpp"
#include "core.hpp"

listener_t::listener_t(boost::asio::io_context& io, core_t& core, const boost::asio::ip::tcp::endpoint& endpoint) :
	m_io(io),
    m_core(core),
    m_acceptor(io)
{
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
}

listener_t::~listener_t() {
	LOG << "Server stopped";
}

void listener_t::do_accept() {
    auto this_ptr = shared_from_this();
    m_acceptor.async_accept(boost::asio::make_strand(m_io), [this_ptr](boost::beast::error_code err, boost::asio::ip::tcp::socket socket) {
    	if (err) {
			std::cout << "Error listener accept: " << err.message() << std::endl;
		} else {
			std::make_shared<session_t>(this_ptr->m_io, this_ptr->m_core, std::move(socket))->run();
		}
		this_ptr->do_accept();
    });
}

void listener_t::run() {
    do_accept();
}
