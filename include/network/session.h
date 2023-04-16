#ifndef SESSION_H
#define SESSION_H
#include <memory>
#include <boost/asio.hpp>
#include "data/data_block_handler.h"


namespace network {

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket, std::shared_ptr<DataBlockHandler> data_block_handler);
	void start();

private:
	void do_read();
	void deserialize_vector_part(std::stringstream& ss, std::vector<size_t>& v);

private:
	tcp::socket m_Socket;
	enum { max_length = 65535 };
	char m_Data[max_length];
	std::shared_ptr<DataBlockHandler> m_DataBlockHandler;
};

}
#endif // SESSION_H
