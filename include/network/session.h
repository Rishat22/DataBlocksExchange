#ifndef SESSION_H
#define SESSION_H
#include <memory>
#include <boost/asio.hpp>


namespace network {

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(tcp::socket socket);
	void start();

private:
	void do_read();
	void deserialize_vector_part(std::stringstream& ss, std::vector<size_t>& v);

	void connect();
	void disconnect();
private:
	tcp::socket m_Socket;
	enum { max_length = 65535 };
	char m_Data[max_length];
};

}
#endif // SESSION_H
