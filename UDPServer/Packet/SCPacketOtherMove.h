#ifndef _SC_PACKET_OTHER_MOVE_H_
#define _SC_PACKET_OTHER_MOVE_H_

#include "UDPPacket.h"

class SCPacketOtherMove : public UDPPacket
{
	typedef UDPPacket base;
public:
	void write(char* buffer, int bufferSize, int& index) override;
public:
	int mPlayerID = 0;
	float mPosX = 0.0f;
	float mPosY = 0.0f;
};

#endif