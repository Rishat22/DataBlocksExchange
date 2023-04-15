#include <iostream> // ToDo delete
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

void Session::deserialize_vector_part(std::stringstream& ss, std::vector<size_t>& v)
{
	size_t total_size;
	ss.read(reinterpret_cast<char*>(&total_size), sizeof(total_size));
	if(v.empty())
		v.resize(total_size);

	size_t offset, size;
	ss.read(reinterpret_cast<char*>(&offset), sizeof(offset));
	ss.read(reinterpret_cast<char*>(&size), sizeof(size));

	/** ToDo a better solution, but it didn't work out,
	there wasn't enough time to figure it out */
//	ss.read(reinterpret_cast<char*>(v.data()), size * sizeof(size_t));

	const size_t max_size = (offset + size);
	for(size_t index(offset); index < max_size; ++index)
	{
		ss.read(reinterpret_cast<char*>(&v[index]), sizeof(size_t));
	}
	std::cout << v[offset] << " and " << v[offset + size - 1] << std::endl; // ToDo delete
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
			return;
		}


		std::stringstream ss;
		ss << std::string{m_Data, length};
		std::vector<size_t> gen_vec;
		deserialize_vector_part(ss, gen_vec);

//		async::receive(m_Data, length, m_Context);

		do_read();
	});
}

}
