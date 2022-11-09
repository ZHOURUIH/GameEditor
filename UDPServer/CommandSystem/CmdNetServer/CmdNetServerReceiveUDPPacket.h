#ifndef _CMD_NET_SERVER_RECEIVE_UDP_PACKET_H_
#define _CMD_NET_SERVER_RECEIVE_UDP_PACKET_H_

#include "GameCommand.h"
#include "Map.h"
#include "Vector.h"

class UDPPacket;
// 通知服务器收到了UDP的消息包
class CmdNetServerReceiveUDPPacket : public GameCommand
{
public:
	void execute() override;
public:
	sockaddr_in mAddress;
	Vector<UDPPacket*> mPacketList;
};

#endif