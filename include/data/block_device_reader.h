#ifndef BLOCKDEVICEREADER_H
#define BLOCKDEVICEREADER_H
#include <string>
#include "idevice_reader.h"

class BlockDeviceReader : public IDeviceReader
{
public:
	BlockDeviceReader();

	size_t get_block_number(const std::string& hash) override;
	size_t get_block_size(const std::string& hash) override;
	int get_block_data(size_t block_num, char* buffer, size_t buffer_size) override;
private:
};

#endif // BLOCKDEVICEREADER_H
