#ifndef _SOCKET_PACKET_H_
#define _SOCKET_PACKET_H_

#include "CommonDefine.h"
#include "txSerializedData.h"
#include "ToolCoreBase.h"

class SocketPacket : public ToolCoreBase, public txSerializedData
{
public:
	SocketPacket(const SOCKET_PACKET& type)
		:
		mType(type)
	{}
	const SOCKET_PACKET& getPacketType() { return mType; }
	virtual void execute() = 0;
protected:
	SOCKET_PACKET mType;
};

#endif