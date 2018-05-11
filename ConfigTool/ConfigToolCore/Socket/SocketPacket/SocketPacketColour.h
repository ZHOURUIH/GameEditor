#ifndef _SOCKET_PACKET_COLOUR_H_
#define _SOCKET_PACKET_COLOUR_H_

#include "SocketPacket.h"

#ifdef _USE_SOCKET_UDP

// 发送的阻力消息
class SocketPacketColour : public SocketPacket
{
public:
	SocketPacketColour(SOCKET_PACKET type)
		:
		SocketPacket(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute(){}
	virtual void fillParams()
	{
		pushArrayParam(mData, 8, "data");
	}
	// 颜色RGB
	void setColour(char r, char g, char b);
protected:
	unsigned char mData[8];
};

#endif

#endif