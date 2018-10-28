#ifndef _SOCKET_PACKET_FRICTION_RET_H_
#define _SOCKET_PACKET_FRICTION_RET_H_

#include "SocketPacket.h"

// 接收到的阻力回复
class SocketPacketFrictionRet : public SocketPacket
{
public:
	SocketPacketFrictionRet(const SOCKET_PACKET& type)
		:
		SocketPacket(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushArrayParam(mData, 6, "data");
	}
protected:
	unsigned char mData[6];
};

#endif