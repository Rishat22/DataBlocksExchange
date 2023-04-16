#ifndef ASYNCCLIENT_H
#define ASYNCCLIENT_H
#include <boost/asio.hpp>
#include <vector>

namespace ba = boost::asio;

class AsyncClient
{
public:
	AsyncClient();

	void get(const std::string& host, const std::string& service);
	void run();
	void generate_request();
private:
	void resolve_handler(const boost::system::error_code &ec, ba::ip::tcp::resolver::iterator it);
	void connect_handler(const boost::system::error_code &ec);
	void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
	void do_read();
	void serialize_vector_part(std::stringstream& ss, std::vector<size_t> const& v, const size_t offset, const size_t size);
private:
	ba::io_context m_IoContext;
	ba::ip::tcp::resolver m_Resolver;
	ba::ip::tcp::socket m_TCPSocket;
	enum { max_length = 65535 };
	std::array<char, max_length> m_Buffer;
	std::vector<size_t> m_ReqHashesList;
};

#endif // ASYNCCLIENT_H
