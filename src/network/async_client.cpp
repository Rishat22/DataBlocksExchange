#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>

#include "async_client.h"
namespace network {

AsyncClient::AsyncClient()
	: m_Resolver{m_IoContext}
	, m_TCPSocket{m_IoContext}
{
}

void AsyncClient::get(const std::string& host, const std::string& service)
{
	ba::ip::tcp::resolver::query query{host, service};

	m_Resolver.async_resolve( query,
	[this](const boost::system::error_code &ec, ba::ip::tcp::resolver::iterator it){
		this->resolve_handler(ec, it);
	});
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

	m_TCPSocket.async_connect(*it,
	[&](const boost::system::error_code &ec){
		this->connect_handler(ec);
	});
}

void AsyncClient::connect_handler(const boost::system::error_code& ec)
{
	if (ec)
	{
		std::cout << "connectHandler - failed! err = " << ec.message() << std::endl;
		return;
	}

	generate_request();

	do_read();
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
	});
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

//ToDo return back
//	const size_t max_possible_len = 10000;
	const size_t len = 3550;//std::rand() % max_possible_len;

	m_ReqHashesList = std::vector<size_t>(len);
	for(size_t index(0); index < len; ++index)
	{
		m_ReqHashesList[index] = index;
	}

	const size_t max_packet_len = 1000;
	send_request_hashes(m_ReqHashesList, max_packet_len, 0);
}

/** We send requests in parts.
 *  The separation occurs according to the max_packet_len rule */
void AsyncClient::send_request_hashes(const std::vector<size_t>& v, const size_t max_packet_len, const size_t offset)
{
	const auto total_size = v.size();
	if(offset >= total_size)
	{
		return;
	}
	size_t packet_size = offset + max_packet_len;
	packet_size = (packet_size < total_size) ? max_packet_len : (total_size - offset);

	std::stringstream ss;
	serialize_vector_part(ss, v, offset, packet_size);
	ba::async_write(m_TCPSocket, ba::buffer(ss.str()),
	[this, &v, max_packet_len, offset, packet_size](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			send_request_hashes(v, max_packet_len, offset + packet_size);
		}
	});
}

/** ToDo there are no different error checks */
void AsyncClient::serialize_vector_part(std::stringstream& ss, const std::vector<size_t>& v, const size_t offset, const size_t size)
{
	const auto total_size = v.size();
	ss.write(reinterpret_cast<char const*>(&total_size), sizeof(total_size));
	ss.write(reinterpret_cast<char const*>(&offset), sizeof(offset));
	ss.write(reinterpret_cast<char const*>(&size), sizeof(size));

	/** ToDo a better solution, but it didn't work,
	there wasn't enough time to figure it out */
//	os.write(reinterpret_cast<char const*>(v.data() + offset * sizeof(size_t) ), v.size() * sizeof(size_t));

	const size_t max_size = (offset + size);
	for(size_t index(offset); index < max_size; ++index)
	{
		ss.write(reinterpret_cast<char const*>(&v[index]), sizeof(size_t));
	}
}

}
