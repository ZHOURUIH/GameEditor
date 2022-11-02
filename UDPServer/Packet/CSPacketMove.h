#ifndef _CS_PACKET_MOVE_H_
#define _CS_PACKET_MOVE_H_

#include "UDPPacket.h"

class CSPacketMove : public UDPPacket
{
	typedef UDPPacket base;
public:
	void read(char* buffer, int bufferSize, int& index) override;
	void execute(const sockaddr_in& addr) override;
public:
	int mPlayerID = 0;
	float mPosX = 0.0f;
	float mPosY = 0.0f;
};

#endif