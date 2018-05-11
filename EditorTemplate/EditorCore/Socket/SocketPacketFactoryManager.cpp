#include "SocketPacketFactoryManager.h"
#include "SocketPacketFactory.h"
#include "EditorCoreLog.h"

void SocketPacketFactoryManager::init()
{
	if (mFactoryList.size() < SP_MAX)
	{
		EDITOR_CORE_ERROR("error : not all udp packet registered! register count : %d, max count : %d", mFactoryList.size(), SP_MAX);
	}
}