#include <iostream> // ToDo delete
#include "session.h"

namespace network {

Session::Session(tcp::socket socket, std::shared_ptr<DataBlockHandler> data_block_handler)
	: m_Socket(std::move(socket))
	, m_DataBlockHandler(data_block_handler)
{

}

void Session::start()
{
	do_read();
}

void Session::do_read()
{
	auto self(shared_from_this());
	m_Socket.async_read_some(boost::asio::buffer(m_Data, max_length),
	[this, self](boost::system::error_code ec, std::size_t length)
	{
		if (ec)
		{
			/** disconnect */
			std::cout << ec.message() << std::endl;
			return;
		}

		std::stringstream ss;
		ss << std::string{m_Data, length};
		std::vector<size_t> req_hashes;
		deserialize_vector_part(ss, req_hashes);

	/** I made the logic of working in a separate thread
	 *  for a non-blocking call to the main thread,
	 *  but I didn't think of how to beautifully return the data
	 *  to be sent to the network. */
//		m_DataBlockHandler->request_data(req_hashes);

	/** Therefore, for now I will just make a blocking call
	 *  to the data request. */
		m_DataBlockHandler->request_data_from_device_reader(req_hashes);

		do_read();
	});
}

/** ToDo there are no different error checks */
void Session::deserialize_vector_part(std::stringstream& ss, std::vector<size_t>& v)
{
	size_t total_size;
	ss.read(reinterpret_cast<char*>(&total_size), sizeof(total_size));

	size_t offset, size;
	ss.read(reinterpret_cast<char*>(&offset), sizeof(offset));
	ss.read(reinterpret_cast<char*>(&size), sizeof(size));

	/** ToDo a better solution, but it didn't work out,
	there wasn't enough time to figure it out */
//	ss.read(reinterpret_cast<char*>(v.data()), size * sizeof(size_t));

	v.resize(size);
	for(size_t index(0); index < size; ++index)
	{
		ss.read(reinterpret_cast<char*>(&v[index]), sizeof(size_t));
	}
}

}
