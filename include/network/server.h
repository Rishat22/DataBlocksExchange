#ifndef SERVER_H
#define SERVER_H
#include <boost/asio.hpp>
#include "block_device_reader.h"

namespace network {

using boost::asio::ip::tcp;

class Server
{
public:
	Server(const short port, const size_t commands_size);
	Server(const boost::asio::ip::address& addr, const short port, const size_t commands_size);
	void run();

private:
	void do_accept();

private:
	boost::asio::io_context m_IoContext;
	tcp::acceptor m_Acceptor;
	size_t m_CommandsSize;
	BlockDeviceReader* m_BlockDeviceReader;
};


}
#endif // SERVER_H
