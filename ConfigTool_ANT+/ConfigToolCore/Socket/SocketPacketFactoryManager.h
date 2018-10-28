#ifndef _SOCKET_PACKET_FACTORY_MANAGER_H_
#define _SOCKET_PACKET_FACTORY_MANAGER_H_

#include "txFactoryManager.h"

#include "CommonDefine.h"

class SocketPacketFactoryBase;
class SocketPacketFactoryManager : public txFactoryManager<SOCKET_PACKET, SocketPacketFactoryBase>
{
public:
	virtual void init();
	static SOCKET_PACKET checkPacketType(char* buffer, const int& bufferLen);
};

#endif