#include <iostream>
#include <string>
#include <thread>

#include "async_client.h"

AsyncClient::AsyncClient()
	: m_Resolver{m_IoContext}
	, m_TCPSocket{m_IoContext}
{
}

void AsyncClient::get(const std::string& host, const std::string& service)
{
	ba::ip::tcp::resolver::query query{host, service};
	m_Resolver.async_resolve(
				query,
				[this](const boost::system::error_code &ec, ba::ip::tcp::resolver::iterator it) {
		this->resolve_handler(ec, it);
	}
	);
}

void AsyncClient::run()
{
	m_IoContext.run();
}

void AsyncClient::resolve_handler(const boost::system::error_code& ec, ba::ip::tcp::resolver::iterator it)
{
	if (ec) {
		std::cout << "resolveHandler - failed! err = " << ec.message() << std::endl;
		return;
	}

	if (it == ba::ip::tcp::resolver::iterator{}) {
		std::cout << "resolveHandler - no address!" << std::endl;
		return;
	}

	m_TCPSocket.async_connect(
				*it,
				[&](const boost::system::error_code &ec) {
		this->connect_handler(ec);
	}
	);
}

void AsyncClient::connect_handler(const boost::system::error_code& ec)
{
	if (ec) {
		std::cout << "connectHandler - failed! err = " << ec.message() << std::endl;
		return;
	}

	const std::string request = "GET /notExists HTTP/1.1\r\n\r\n";
	ba::write(m_TCPSocket, ba::buffer(request));

	m_TCPSocket.async_read_some(
				ba::buffer(m_Buffer),
				[&](const boost::system::error_code &ec, std::size_t bytes_transferred) {
		this->read_handler(ec, bytes_transferred);
	}
	);
}

void AsyncClient::read_handler(const boost::system::error_code& ec, std::size_t bytes_transferred) {
	if (ec) {
		std::cout << "readHandler - failed! err = " << ec.message() << std::endl;
		return;
	}

	if (!bytes_transferred) {
		std::cout << "readHandler - no more data!" << std::endl;
		return;
	}

	std::cout << "[readHandler] - read " << bytes_transferred << " bytes" << std::endl;

	std::cout.write(m_Buffer.data(), bytes_transferred);
	m_TCPSocket.async_read_some(
				ba::buffer(m_Buffer),
				[&](const boost::system::error_code &ec, std::size_t bytes_transferred) {
		read_handler(ec, bytes_transferred);
	}
	);
}
