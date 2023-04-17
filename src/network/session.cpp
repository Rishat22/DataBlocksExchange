#include <iostream>
#include "data/data_serialization.h"
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
		serialization::deserialize_vector_part(ss, req_hashes);

	/** I made the logic of working in a separate thread
	 *  for a non-blocking call to the main thread,
	 *  but I didn't think of how to beautifully return the data
	 *  to be sent to the network. */
//		m_DataBlockHandler->request_data(req_hashes);

	/** Therefore, for now I will just make a blocking call
	 *  to the data request. */
		const auto res_data_blocks = m_DataBlockHandler->request_data_from_device_reader(req_hashes);
		send_data_blocks(res_data_blocks);
	});
}

void Session::send_data_blocks(const std::vector<DataBlock>& data_blocks)
{
	for(const auto& data_block : data_blocks)
	{
		size_t max_packet_len = 10000;
		max_packet_len = (max_packet_len < data_block.m_Size) ? max_packet_len : data_block.m_Size;
		send_data_block(data_block, 0, max_packet_len);
	}

	do_read();
}

void Session::send_data_block(const DataBlock& data_block, const size_t offset, const size_t size)
{
	if(offset >= data_block.m_Size)
	{
		return;
	}

	size_t packet_size = offset + size;
	packet_size = (packet_size < data_block.m_Size) ? packet_size : (data_block.m_Size - offset);

	std::stringstream ss;
	serialization::serialize_data_block(ss, data_block, offset, size);
	boost::asio::async_write(m_Socket, boost::asio::buffer(ss.str()),
	[this, &data_block, offset, packet_size](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			send_data_block(data_block, offset + packet_size, packet_size);
		}
	});
}

}
