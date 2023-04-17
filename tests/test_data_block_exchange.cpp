#define BOOST_TEST_MODULE unit_tests

#include <iostream>
#include <string>
#include <list>
#include <filesystem>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "data/data_block.h"
#include "data/data_block_handler.h"
#include "data/block_device_reader.h"
#include "data/data_serialization.h"


BOOST_AUTO_TEST_SUITE(unit_tests)

BOOST_AUTO_TEST_CASE(test_check_DataBlock)
{
	size_t hash = 1234;
	DataBlock block(hash, nullptr, 0);

	BOOST_CHECK(block.m_Hash == hash);
	BOOST_CHECK(block.m_Data == nullptr);
	BOOST_CHECK(block.m_Size == 0);
}

BOOST_AUTO_TEST_CASE(test_check_DataBlockHandler)
{
	DataBlockHandler block_handler(std::make_shared<BlockDeviceReader>());
	block_handler.start_work();

	std::vector<size_t> req_hashes_list = {1,2,3,4,5,6};
	const auto res = block_handler.request_data_from_device_reader(req_hashes_list);

	BOOST_CHECK(req_hashes_list.size() == res.size());
}

BOOST_AUTO_TEST_CASE(test_check_serialization)
{
	std::vector<size_t> test_values = {1,2,3,4,5,6};
	std::stringstream ss;
	serialization::serialize_vector_part(ss, test_values, 0, test_values.size());

	std::vector<size_t> res;
	serialization::deserialize_vector_part(ss, res);

	BOOST_CHECK(test_values == res);
}
BOOST_AUTO_TEST_SUITE_END()
