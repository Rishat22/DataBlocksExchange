#ifndef BLOCKDEVICEREADER_H
#define BLOCKDEVICEREADER_H
#include <string>

class BlockDeviceReader
{
public:
	BlockDeviceReader();

	size_t get_block_number(const std::string& hash);
	size_t get_block_size(const std::string& hash);
	int get_block_data(size_t block_num, char* buffer, size_t buffer_size);
private:
};

#endif // BLOCKDEVICEREADER_H
