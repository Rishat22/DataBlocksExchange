#include "console_output.h"
#include <ctime>
#include <iostream>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ostream>
#include <thread>
#include <vector>

void ConsoleOutput::print(const std::vector<std::string>& str_data)
{
	std::thread([]
	(const std::vector<std::string>& str_data)
	{
		std::cout << "bulk: ";
		size_t index = 0;
		for(const auto& command_data : str_data)
		{
			std::cout << command_data;
			if(++index < str_data.size())
				std::cout << ", ";
		}
		std::cout << std::endl;
	},	std::cref(str_data) ).join();
}
