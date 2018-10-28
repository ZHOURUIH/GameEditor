#include "ANTPlusPacketRidingSpeed.h"
#include "Utility.h"
#include "LogSystem.h"
#include "ANTPlusDataRidingSpeed.h"

ANTPlusPacketRidingSpeed::ANTPlusPacketRidingSpeed(ANT_PLUS_PACKET_TYPE type)
:ANTPlusPacket(type)
{
	mDataRidingSpeed = NULL;
}

void ANTPlusPacketRidingSpeed::checkANTPlusData()
{
	if (mDataRidingSpeed == NULL)
	{
		mDataRidingSpeed = TRACE_NEW(ANTPlusDataRidingSpeed, mDataRidingSpeed, this);
		mData = mDataRidingSpeed;
	}
}