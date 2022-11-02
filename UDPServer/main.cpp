#include <iostream>
#include "UDPServer.h"

int main()
{
	UDPServer server;
	server.connect(11112);
	while (true)
	{
		server.update();
	}
	server.close();
	return 0;
}