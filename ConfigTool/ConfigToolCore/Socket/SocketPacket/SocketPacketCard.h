#ifndef _SOCKET_PACKET_CARD_H_
#define _SOCKET_PACKET_CARD_H_

#include "SocketPacket.h"

#ifdef _USE_SOCKET_UDP

// Ë¢¿¨µÇÂ¼
class SocketPacketCard : public SocketPacket
{
public:
	SocketPacketCard(SOCKET_PACKET type)
		:
		SocketPacket(type)
	{
		fillParams();
		zeroParams();
	}
	virtual void execute();
	virtual void fillParams()
	{
		pushArrayParam(mData, 21, "data");
	}
protected:
	unsigned char mData[21];
};

#endif

#endif