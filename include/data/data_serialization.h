#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include <cstddef>
#include <string>
#include <vector>
#include <sstream>
#include "data/data_block.h"

namespace serialization
{

void serialize_data_block(std::stringstream& ss, const DataBlock& data_block, const size_t offset, const size_t size);

void deserialize_vector_part(std::stringstream& ss, std::vector<size_t>& v);
void serialize_vector_part(std::stringstream& ss, const std::vector<size_t>& v, const size_t offset, const size_t size);

}

#endif // SERIALIZATION_H
