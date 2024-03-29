#include <memory>
#include "session.h"
#include "server.h"

namespace network {

Server::Server(const short port)
	: m_Acceptor(m_IoContext, tcp::endpoint(tcp::v4(), port))
{
	std::shared_ptr<IDeviceReader> m_BlockDeviceReader = std::make_shared<BlockDeviceReader>();
	m_DataBlockHandler = std::make_shared<DataBlockHandler>(m_BlockDeviceReader);
	m_DataBlockHandler->start_work();
	do_accept();
}

Server::Server(const boost::asio::ip::address& addr, const short port)
	: m_Acceptor(m_IoContext, tcp::endpoint(addr, port))
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
			std::make_shared<Session>(std::move(socket), m_DataBlockHandler)->start();
		}

		do_accept();
	});
}


}
