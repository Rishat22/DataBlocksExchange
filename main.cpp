#include <iostream>
#include "server.h"

int main(int argc, char* argv[])
{
	short port;
	if (argc != 2)
	{
		std::cerr << "Usage: DataBlocksExchange <port>\n";
		return 1;
	}
	try
	{
		port = std::stoi(argv[1]);
	}
	catch (const std::invalid_argument& ia) {
		std::cerr << "Invalid argument: " << ia.what() << '\n';
		return 1;
	}

	network::Server server(port);
	server.run();

  return 0;
}
