#ifndef ASYNCCLIENT_H
#define ASYNCCLIENT_H
#include <boost/asio.hpp>
#include <array>

namespace ba = boost::asio;

class AsyncClient
{
public:
	AsyncClient();

	void get(const std::string& host, const std::string& service);
	void run();
private:
	void resolve_handler(const boost::system::error_code &ec, ba::ip::tcp::resolver::iterator it);
	void connect_handler(const boost::system::error_code &ec);
	void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);

private:
	ba::io_context m_IoContext;
	ba::ip::tcp::resolver m_Resolver;
	ba::ip::tcp::socket m_TCPSocket;
	std::array<char, 4096> m_Buffer;

};

#endif // ASYNCCLIENT_H
