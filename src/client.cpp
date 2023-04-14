#include <iostream>
#include "async_client.h"

int main()
{
	AsyncClient client;
	client.get("localhost", "9000");

    return 0;
}





