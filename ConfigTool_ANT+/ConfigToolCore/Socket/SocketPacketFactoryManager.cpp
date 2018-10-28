#include "SocketPacketFactoryManager.h"
#include "SocketPacketFactory.h"
#include "SocketPacketFriction.h"
#include "SocketPacketSpeed.h"
#include "SocketPacketFrictionRet.h"
#include "SocketPacketHeartRate.h"
#include "SocketPacketColour.h"
#include "ToolCoreLog.h"
#include "ToolCoreUtility.h"

void SocketPacketFactoryManager::init()
{
	addFactory<SocketPacketFactory<SocketPacketFriction> >(SP_FRICTION);
	addFactory<SocketPacketFactory<SocketPacketSpeed> >(SP_SPEED);
	addFactory<SocketPacketFactory<SocketPacketFrictionRet> >(SP_FRICTION_RET);
	addFactory<SocketPacketFactory<SocketPacketHeartRate> >(SP_HEART_RATE);
	addFactory<SocketPacketFactory<SocketPacketColour> >(SP_COLOUR);
	if (mFactoryList.size() < SP_MAX)
	{
		TOOL_CORE_ERROR("error : not all udp packet registered! register count : %d, max count : %d", mFactoryList.size(), SP_MAX);
	}
}

SOCKET_PACKET SocketPacketFactoryManager::checkPacketType(char* buffer, const int& bufferLen)
{
	if (bufferLen < 2)
	{
		return SP_MAX;
	}
	if (buffer[1] == 0)
	{
		return SP_SPEED;
	}
	else if (buffer[1] == 2)
	{
		return SP_FRICTION_RET;
	}
	else if (buffer[1] == 3)
	{
		return SP_FRICTION;
	}
	else if (buffer[1] == 5)
	{
		return SP_HEART_RATE;
	}
	else if (buffer[1] == 6)
	{
		return SP_COLOUR;
	}
	return SP_MAX;
}