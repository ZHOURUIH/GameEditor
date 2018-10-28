#ifndef _SOCKET_PACKET_FACTORY_H_
#define _SOCKET_PACKET_FACTORY_H_

#include "SocketPacket.h"

#include "Utility.h"
#include "ToolCoreUtility.h"
#include "ToolCoreLog.h"
#include "txMemoryTrace.h"

class SocketPacketFactoryBase
{
public:
	SocketPacketFactoryBase(SOCKET_PACKET type)
		:
		mType(type)
	{}
	virtual ~SocketPacketFactoryBase(){}
	virtual SocketPacket* createPacket() = 0;
	void destroyPacket(SocketPacket* packet)
	{
		TRACE_DELETE(packet);
	}
	SOCKET_PACKET getType() { return mType; }
	template<typename T>
	static SocketPacketFactoryBase* createFactory(SOCKET_PACKET type)
	{
		T* factory = TRACE_NEW(T, factory, type);
		return factory;
	}
protected:
	SOCKET_PACKET mType;
};

template <class T>
class SocketPacketFactory : public SocketPacketFactoryBase
{
public:
	SocketPacketFactory(SOCKET_PACKET type)
		:
		SocketPacketFactoryBase(type)
	{}

	SocketPacket* createPacket()
	{
		T* newPacket = TRACE_NEW(T, newPacket, mType);
		return newPacket;
	}
};

#endif