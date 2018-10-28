#include "ANTPlusPacketRidingCadence.h"
#include "Utility.h"
#include "LogSystem.h"
#include "ANTPlusDataRidingCadence.h"

ANTPlusPacketRidingCadence::ANTPlusPacketRidingCadence(ANT_PLUS_PACKET_TYPE type)
:ANTPlusPacket(type)
{
	mDataRidingCadence = NULL;
}

void ANTPlusPacketRidingCadence::checkANTPlusData()
{
	if (mDataRidingCadence == NULL)
	{
		mDataRidingCadence = TRACE_NEW(ANTPlusDataRidingCadence, mDataRidingCadence, this);
		mData = mDataRidingCadence;
	}
}

void ANTPlusPacketRidingCadence::writeReceiveLog()
{
	std::string info;
	info += "Receive : FullBytes : ";
	info += StringUtility::charArrayToHexString(mBuffer, mBufferLength);
	info += ", MachineIndex : ";
	info += StringUtility::intToString(mDataRidingCadence->mMachineIndex) + ".";
	mLogSystem->log(info);
}

void ANTPlusPacketRidingCadence::writeUpdateLog()
{
	std::string info;
	info += "Update : FullBytes : ";
	info += StringUtility::charArrayToHexString(mBuffer, mBufferLength);
	info += ", MachineIndex : ";
	info += StringUtility::intToString(mDataRidingCadence->mMachineIndex);
	info += ", Cadence : ";
	info += StringUtility::intToString(mDataRidingCadence->getCadence());
	mLogSystem->log(info);
}