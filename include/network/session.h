#ifndef SESSION_H
#define SESSION_H
#include <memory>
#include <boost/asio.hpp>


namespace network {

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket, const size_t commands_size);
    void start();

private:
    void do_read();

private:
    tcp::socket m_Socket;
    enum { max_length = 1024 };
    char m_Data[max_length];
    int m_Context;
};

}
#endif // SESSION_H
