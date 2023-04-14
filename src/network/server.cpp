#include <memory>
#include "session.h"
#include "server.h"

namespace network {

Server::Server(const short port, const size_t commands_size)
	: m_Acceptor(m_IoContext, tcp::endpoint(tcp::v4(), port))
	, m_CommandsSize(commands_size)
{
	do_accept();
}

Server::Server(const boost::asio::ip::address& addr, const short port, const size_t commands_size)
	: m_Acceptor(m_IoContext, tcp::endpoint(addr, port))
	, m_CommandsSize(commands_size)
{
	do_accept();
}

void Server::run()
{
	m_IoContext.run();
}

void Server::do_accept()
{
	m_Acceptor.async_accept(
	[this](const boost::system::error_code ec, tcp::socket socket)
	{
		if (!ec)
		{
			std::make_shared<Session>(std::move(socket), m_CommandsSize)->start();
		}

		do_accept();
	});
}


}
