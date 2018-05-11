#include "SocketPacketFactoryManager.h"
#include "SocketPacketFactory.h"
#include "SocketPacketFriction.h"
#include "SocketPacketSpeed.h"
#include "SocketPacketFrictionRet.h"
#include "SocketPacketCard.h"
#include "SocketPacketHeartRate.h"
#include "SocketPacketColour.h"
#include "ToolCoreLog.h"

void SocketPacketFactoryManager::init()
{
	addFactory<SocketPacketFactory<SocketPacketFriction> >(SP_FRICTION);
	addFactory<SocketPacketFactory<SocketPacketSpeed> >(SP_SPEED);
	addFactory<SocketPacketFactory<SocketPacketFrictionRet> >(SP_FRICTION_RET);
	addFactory<SocketPacketFactory<SocketPacketCard> >(SP_CARD);
	addFactory<SocketPacketFactory<SocketPacketHeartRate> >(SP_HEART_RATE);
	addFactory<SocketPacketFactory<SocketPacketColour> >(SP_COLOUR);
	if (mFactoryList.size() < SP_MAX)
	{
		TOOL_CORE_ERROR("error : not all udp packet registered! register count : %d, max count : %d", mFactoryList.size(), SP_MAX);
	}
}