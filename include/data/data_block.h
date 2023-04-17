#ifndef DATABLOCK_H
#define DATABLOCK_H
#include <cstddef>

class DataBlock
{
public:
	DataBlock()
		: m_Hash(0)
		, m_Data(nullptr)
		, m_Size(0)
	{
	}
	DataBlock(size_t hash, char* newData, int size)
		: m_Hash(hash)
		, m_Data(newData)
		, m_Size(size)
	{
	}
	~DataBlock() { if(m_Data) delete m_Data;}
public:
	size_t m_Hash;
	char* m_Data;
	int m_Size;
};



#endif // DATABLOCK_H
