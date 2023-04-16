#include <iostream>
#include "network/async_client.h"

int main(int argc, char* argv[])
{
	std::string host;
	std::string service;
	if (argc != 3)
	{
		std::cerr << "Usage: client <host> <service>\n";
		return 1;
	}
	try
	{
		host = std::string(argv[1]);
		service = std::string(argv[2]);
	}
	catch (const std::invalid_argument& ia) {
		std::cerr << "Invalid argument: " << ia.what() << '\n';
		return 1;
	}

	AsyncClient client;
	client.get(host, service);
	client.run();

    return 0;
}





