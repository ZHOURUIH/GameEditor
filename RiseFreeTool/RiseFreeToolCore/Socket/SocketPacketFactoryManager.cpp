#include "SocketPacketFactoryManager.h"
#include "SocketPacketFactory.h"
#include "SocketPacketFriction.h"
#include "SocketPacketSpeed.h"
#include "SocketPacketFrictionRet.h"
#include "EditorCoreLog.h"

void SocketPacketFactoryManager::init()
{
	addFactory<SocketPacketFactory<SocketPacketFriction> >(SP_FRICTION);
	addFactory<SocketPacketFactory<SocketPacketSpeed> >(SP_SPEED);
	addFactory<SocketPacketFactory<SocketPacketFrictionRet> >(SP_FRICTION_RET);
	if (mFactoryList.size() < SP_MAX)
	{
		EDITOR_CORE_ERROR("error : not all udp packet registered! register count : %d, max count : %d", mFactoryList.size(), SP_MAX);
	}
}