#ifndef _SOCKET_PACKET_SPEED_H_
#define _SOCKET_PACKET_SPEED_H_

#include "SocketPacket.h"

class SocketPacketSpeed : public SocketPacket
{
public:
	SocketPacketSpeed(const SOCKET_PACKET& type)
		:
		SocketPacket(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushArrayParam(mData, 12, "data");
	}
protected:
	unsigned char mData[12];
};

#endif