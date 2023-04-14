#ifndef DATABLOCK_H
#define DATABLOCK_H
#include <string>

class DataBlock
{

public:
	char* Data() const;
	void setData(char* newData, int size);

	const std::string& Hash() const;
	void setHash(const std::string& newHash);

private:
	char* m_Data;
	int m_Size;
	std::string m_Hash;
};

inline char* DataBlock::Data() const
{
	return m_Data;
}

inline void DataBlock::setData(char* newData, int size)
{
	m_Data = newData;
	m_Size = size;
}

inline const std::string& DataBlock::Hash() const
{
	return m_Hash;
}

inline void DataBlock::setHash(const std::string& newHash)
{
	m_Hash = newHash;
}

#endif // DATABLOCK_H
