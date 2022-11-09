#include "CmdNetServerReceiveUDPPacket.h"
#include "UDPPacket.h"
#include "UDPServer.h"

void CmdNetServerReceiveUDPPacket::execute()
{
	UDPServer::get()->addClient(mAddress);
	for(auto iter : mPacketList)
	{
		iter->execute(mAddress.sin_port);
		delete iter;
	}
	mPacketList.clear();
}