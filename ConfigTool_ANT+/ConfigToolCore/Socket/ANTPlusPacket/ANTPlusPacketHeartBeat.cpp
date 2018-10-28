#include "ANTPlusPacketHeartBeat.h"
#include "Utility.h"
#include "txSerializer.h"
#include "ANTPlusData.h"
#include "ToolCoreBase.h"
#include "LogSystem.h"
#include "ANTPlusDataHeartBeat.h"

ANTPlusPacketHeartBeat::ANTPlusPacketHeartBeat(ANT_PLUS_PACKET_TYPE type)
:ANTPlusPacket(type)
{
	mDataHeartBeat = NULL;
}

void ANTPlusPacketHeartBeat::checkANTPlusData()
{
	if (mDataHeartBeat == NULL)
	{
		mDataHeartBeat = TRACE_NEW(ANTPlusDataHeartBeat, mDataHeartBeat, this);
		mData = mDataHeartBeat;
	}
}