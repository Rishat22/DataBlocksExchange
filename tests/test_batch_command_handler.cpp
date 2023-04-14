#define BOOST_TEST_MODULE unit_tests

#include <iostream>
#include <string>
#include <list>
#include <filesystem>
#include <thread>
#include <async.h>
#include <boost/test/unit_test.hpp>

namespace fs = std::filesystem;

BOOST_AUTO_TEST_SUITE(unit_tests)

BOOST_AUTO_TEST_CASE(test_check_number_files_created)
{
	const auto dir_path = std::filesystem::current_path() / "logs";
	if (fs::directory_entry(dir_path).exists())
	{
		fs::remove_all(dir_path);
	}
	const size_t commands_max_size = 3;
	const auto context = async::connect(commands_max_size);
	std::list<std::string> test_commands{
		std::string("cmd1"),
		std::string("cmd2"),
		std::string("{"),
		std::string("cmd3"),
		std::string("cmd4"),
		std::string("}"),
		std::string("{"),
		std::string("cmd5"),
		std::string("cmd6"),
		std::string("{"),
		std::string("cmd7"),
		std::string("cmd8"),
		std::string("}"),
		std::string("cmd9"),
		std::string("}"),
		std::string("{"),
		std::string("cmd10"),
		std::string("cmd11"),
	};
	for(const auto& inputData : test_commands)
	{
		async::receive(inputData.c_str(), inputData.size(), context);
	}
	async::disconnect(context);

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto dirIter = std::filesystem::directory_iterator(dir_path);
	size_t file_count = std::count_if(begin(dirIter), end(dirIter), [](auto& entry)
	{
		return entry.is_regular_file();
	});
	BOOST_CHECK(file_count == 3);
}
BOOST_AUTO_TEST_SUITE_END()
