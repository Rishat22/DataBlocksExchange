#include <iostream>
#include <async.h>
#include "session.h"

namespace network {

Session::Session(tcp::socket socket, const size_t commands_size)
	: m_Socket(std::move(socket))
	, m_Context(-1)
{
	m_Context = async::connect(commands_size);
}

void Session::start()
{
	do_read();
}

void Session::do_read()
{
	auto self(shared_from_this());
	m_Socket.async_read_some(boost::asio::buffer(m_Data, max_length),
	[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (ec)
		{
			ec.message();
			std::cout << std::endl;
			async::disconnect(m_Context);
		}

		async::receive(m_Data, length, m_Context);

		do_read();
	});
}

}
