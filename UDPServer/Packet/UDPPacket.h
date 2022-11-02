#ifndef _UDP_PACKET_H_
#define _UDP_PACKET_H_

#include "Utility.h"
#include "CommonDefine.h"

class UDPPacket
{
public:
	virtual void read(char* buffer, int bufferSize, int& index);
	virtual void write(char* buffer, int bufferSize, int& index);
	PACKET_TYPE getType() const { return mType; }
	void setType(PACKET_TYPE type) { mType = type; }
	virtual void execute(const sockaddr_in& addr) {}
protected:
	PACKET_TYPE mType = PACKET_TYPE::NONE;
};

#endif