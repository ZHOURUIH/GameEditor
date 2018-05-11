#ifndef _SOCKET_PACKET_H_
#define _SOCKET_PACKET_H_

#include "CommonDefine.h"

#ifdef _USE_SOCKET_UDP

#include <vector>
#include <map>

#include "txSerializedData.h"

#include "EditorCoreBase.h"

class SocketPacket : public EditorCoreBase, public txSerializedData
{
public:
	SocketPacket(SOCKET_PACKET type)
		:
		mType(type)
	{}
	SOCKET_PACKET getPacketType() { return mType; }
	virtual void execute() = 0;
protected:
	SOCKET_PACKET mType;
};

#endif

#endif