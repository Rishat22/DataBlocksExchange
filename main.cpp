#include <iostream>
#include "server.h"

int main(int argc, char* argv[])
{
	short port;
	size_t commands_max_size;
	if (argc != 3)
	{
		std::cerr << "Usage: BatchCommandHandler <port> <size_commands>\n";
		return 1;
	}
	try
	{
		port = std::stoi(argv[1]);
		commands_max_size = std::stoi(argv[2]);
	}
	catch (const std::invalid_argument& ia) {
		std::cerr << "Invalid argument: " << ia.what() << '\n';
		return 1;
	}

	network::Server server(port, commands_max_size);
	server.run();

  return 0;
}
