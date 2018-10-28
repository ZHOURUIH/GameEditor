#include "ANTPlusPacketRidingCadenceSpeed.h"
#include "Utility.h"
#include "ANTPlusDataRidingCadenceSpeed.h"
#include "LogSystem.h"

ANTPlusPacketRidingCadenceSpeed::ANTPlusPacketRidingCadenceSpeed(ANT_PLUS_PACKET_TYPE type)
:ANTPlusPacket(type)
{
	mDataRidingCadenceSpeed = NULL;
}

void ANTPlusPacketRidingCadenceSpeed::checkANTPlusData()
{
	if (mDataRidingCadenceSpeed == NULL)
	{
		mDataRidingCadenceSpeed = TRACE_NEW(ANTPlusDataRidingCadenceSpeed, mDataRidingCadenceSpeed, this);
		mData = mDataRidingCadenceSpeed;
	}
}