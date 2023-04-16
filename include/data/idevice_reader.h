#ifndef IOUTPUT_H
#define IOUTPUT_H
#include <string>
#include <vector>

class IDeviceReader
{
public:
	virtual ~IDeviceReader() = default;
	virtual size_t get_block_number(const std::string& hash) = 0;
	virtual size_t get_block_size(const std::string& hash) = 0;
	virtual int get_block_data(size_t block_num, char* buffer, size_t buffer_size) = 0;
};
#endif // IOUTPUT_H
