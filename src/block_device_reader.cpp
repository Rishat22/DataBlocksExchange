#include "block_device_reader.h"

BlockDeviceReader::BlockDeviceReader()
{
}

size_t BlockDeviceReader::get_block_number(const std::string& /*hash*/)
{
	return 0;
}

size_t BlockDeviceReader::get_block_size(const std::string& /*hash*/)
{
	return 0;
}

int BlockDeviceReader::get_block_data(size_t /*block_num*/, char* /*buffer*/, size_t /*buffer_size*/)
{
	return 0;
}
