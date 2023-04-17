#include "data_serialization.h"

namespace serialization
{

/** ToDo there are no different error checks */
void serialize_data_block(std::stringstream& ss, const DataBlock& data_block, const size_t offset, const size_t size)
{
	ss.write(reinterpret_cast<char const*>(&data_block.m_Hash), sizeof(data_block.m_Hash));
	ss.write(reinterpret_cast<char const*>(&data_block.m_Size), sizeof(data_block.m_Size));
	ss.write(reinterpret_cast<char const*>(&offset), sizeof(offset));
	ss.write(reinterpret_cast<char const*>(&size), sizeof(size));

	ss.write(reinterpret_cast<char const*>(data_block.m_Data + offset), size);
}

/** ToDo there are no different error checks */
void deserialize_vector_part(std::stringstream& ss, std::vector<size_t>& v)
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

/** ToDo there are no different error checks */
void serialize_vector_part(std::stringstream& ss, const std::vector<size_t>& v, const size_t offset, const size_t size)
{
	const auto total_size = v.size();
	ss.write(reinterpret_cast<char const*>(&total_size), sizeof(total_size));
	ss.write(reinterpret_cast<char const*>(&offset), sizeof(offset));
	ss.write(reinterpret_cast<char const*>(&size), sizeof(size));

	/** ToDo a better solution, but it didn't work,
	there wasn't enough time to figure it out */
	//	os.write(reinterpret_cast<char const*>(v.data() + offset * sizeof(size_t) ), v.size() * sizeof(size_t));

	const size_t max_size = (offset + size);
	for(size_t index(offset); index < max_size; ++index)
	{
		ss.write(reinterpret_cast<char const*>(&v[index]), sizeof(size_t));
	}
}

}
