#include <iostream>
#include "UDPServer.h"
#include "CommandSystem.h"
#include "TimeLock.h"

int main()
{
	CommandSystem cmdSystem;
	UDPServer server;
	server.connect(11112);
	TimeLock timeLock(15);
	while (true)
	{
		float deltaTime = timeLock.update() * 0.001f;
		cmdSystem.update(deltaTime);
		server.update();
	}
	server.close();
	return 0;
}