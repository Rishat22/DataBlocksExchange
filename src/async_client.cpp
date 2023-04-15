#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>

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
				[this](const boost::system::error_code &ec, ba::ip::tcp::resolver::iterator it)
				{	this->resolve_handler(ec, it); }
	);
}

void AsyncClient::run()
{
	m_IoContext.run();
}

void AsyncClient::resolve_handler(const boost::system::error_code& ec, ba::ip::tcp::resolver::iterator it)
{
	if (ec)
	{
		std::cout << "resolveHandler - failed! err = " << ec.message() << std::endl;
		return;
	}

	if (it == ba::ip::tcp::resolver::iterator{})
	{
		std::cout << "resolveHandler - no address!" << std::endl;
		return;
	}

	m_TCPSocket.async_connect(
				*it,
				[&](const boost::system::error_code &ec)
				{ this->connect_handler(ec); }
	);
}

void AsyncClient::connect_handler(const boost::system::error_code& ec)
{
	if (ec)
	{
		std::cout << "connectHandler - failed! err = " << ec.message() << std::endl;
		return;
	}

	generate_request();

	m_TCPSocket.async_read_some(
				ba::buffer(m_Buffer),
				[&](const boost::system::error_code &ec, std::size_t bytes_transferred) {
		this->read_handler(ec, bytes_transferred);
	}
	);
}

void AsyncClient::read_handler(const boost::system::error_code& ec, std::size_t bytes_transferred)
{
	if (ec)
	{
		std::cout << "readHandler - failed! err = " << ec.message() << std::endl;
		return;
	}

	if (!bytes_transferred)
	{
		std::cout << "readHandler - no more data!" << std::endl;
		return;
	}

	std::cout << "[readHandler] - read " << bytes_transferred << " bytes" << std::endl;


	m_TCPSocket.async_read_some(
				ba::buffer(m_Buffer),
				[&](const boost::system::error_code &ec, std::size_t bytes_transferred) {
		read_handler(ec, bytes_transferred);
	}
	);
}

void AsyncClient::serialize_vector_part(std::stringstream& ss, const std::vector<size_t>& v, const size_t offset, const size_t size)
{
	auto total_size = v.size();
	ss.write(reinterpret_cast<char const*>(&total_size), sizeof(total_size));
	ss.write(reinterpret_cast<char const*>(&offset), sizeof(offset));
	ss.write(reinterpret_cast<char const*>(&size), sizeof(size));

	/** ToDo a better solution, but it didn't work out,
	there wasn't enough time to figure it out */
//	os.write(reinterpret_cast<char const*>(v.data() + offset * sizeof(size_t) ), v.size() * sizeof(size_t));

	const size_t max_size = (offset + size);
	for(size_t index(offset); index < max_size; ++index)
	{
		ss.write(reinterpret_cast<char const*>(&v[index]), sizeof(size_t));
	}
}

void AsyncClient::do_read()
{
	m_TCPSocket.async_read_some(
				ba::buffer(m_Buffer),
				[&](const boost::system::error_code &ec, std::size_t bytes_transferred) {
		this->read_handler(ec, bytes_transferred);
	}
	);
}

void AsyncClient::generate_request()
{
	std::srand(unsigned(std::time(nullptr)));

	const size_t len = 3550;//std::rand() % max_possible_len;

	std::vector<size_t> gen_vec(len);
	for(size_t index(0); index < len; ++index)
	{
		gen_vec[index] = rand();
	}

	const size_t max_packet_len = 1000;
	size_t index(0);
	std::stringstream ss;
	while(index < len)
	{
		ss.str(std::string());

		size_t packet_size = index + max_packet_len;
		packet_size = (packet_size > len) ? (len - index) : max_packet_len;
		serialize_vector_part(ss, gen_vec, index, packet_size);
		std::cout << gen_vec[index] << " and " << gen_vec[index + packet_size] << std::endl;

		ba::async_write(m_TCPSocket, ba::buffer(ss.str()),
		[](boost::system::error_code /*ec*/, std::size_t /*length*/)
		{});

		index += packet_size;
	}
}
