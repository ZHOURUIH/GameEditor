#ifndef _SOCKET_PACKET_SPEED_H_
#define _SOCKET_PACKET_SPEED_H_

#include "SocketPacket.h"

#ifdef _USE_SOCKET_UDP

class SocketPacketSpeed : public SocketPacket
{
public:
	SocketPacketSpeed(SOCKET_PACKET type)
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

#endif