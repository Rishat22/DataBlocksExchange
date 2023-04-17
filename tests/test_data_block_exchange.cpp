#define BOOST_TEST_MODULE unit_tests

#include <iostream>
#include <string>
#include <list>
#include <filesystem>
#include <thread>
#include <boost/test/unit_test.hpp>
#include "data/data_block.h"


BOOST_AUTO_TEST_SUITE(unit_tests)

BOOST_AUTO_TEST_CASE(test_check_data_block)
{
	size_t hash = 1234;
	DataBlock block(hash, nullptr, 0);

	BOOST_CHECK(block.m_Hash == hash);
	BOOST_CHECK(block.m_Data == nullptr);
	BOOST_CHECK(block.m_Size == 0);
}
BOOST_AUTO_TEST_SUITE_END()
