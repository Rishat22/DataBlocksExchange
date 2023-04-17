#include <random>
#include "block_device_reader.h"

BlockDeviceReader::BlockDeviceReader()
{
}


int generate_random_int(int min, int max)
{
	std::random_device rd;     // Only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(min, max); // Guaranteed unbiased

	return uni(rng);
}

size_t BlockDeviceReader::get_block_number(const std::string& /*hash*/)
{
	return generate_random_int(100, 10000);
}

size_t BlockDeviceReader::get_block_size(const std::string& /*hash*/)
{
	return generate_random_int(1, 10000); //ToDo past 1mb as max : 10000000
}

/** Block Device Reader magically fills with data. Fill in the spaces */
int BlockDeviceReader::get_block_data(size_t /*block_num*/, char* buffer, size_t buffer_size)
{
	for (size_t index = 0; index < buffer_size; ++index)
	{
		if (buffer[index] == '\0')
		{
			buffer[index] = ' ';
		}
	}
	return 1;
}
