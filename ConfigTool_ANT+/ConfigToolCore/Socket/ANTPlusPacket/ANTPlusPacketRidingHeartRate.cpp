#include "ANTPlusPacketRidingHeartRate.h"
#include "Utility.h"
#include "LogSystem.h"
#include "ANTPlusDataRidingHeartRate.h"

ANTPlusPacketRidingHeartRate::ANTPlusPacketRidingHeartRate(ANT_PLUS_PACKET_TYPE type)
:ANTPlusPacket(type)
{
	mDataRidingHeartRate = NULL;
}

void ANTPlusPacketRidingHeartRate::checkANTPlusData()
{
	if (mDataRidingHeartRate == NULL)
	{
		mDataRidingHeartRate = TRACE_NEW(ANTPlusDataRidingHeartRate, mDataRidingHeartRate, this);
		mData = mDataRidingHeartRate;
	}
}

void ANTPlusPacketRidingHeartRate::writeReceiveLog()
{
	std::string info;
	info += "Receive : FullBytes : ";
	info += StringUtility::charArrayToHexString(mBuffer, mBufferLength);
	info += ", MachineIndex : ";
	info += StringUtility::intToString(mDataRidingHeartRate->mMachineIndex) + ".";
	ToolCoreBase::mLogSystem->log(info);
}

void ANTPlusPacketRidingHeartRate::writeUpdateLog()
{
	std::string info;
	info += "Update : FullBytes : ";
	info += StringUtility::charArrayToHexString(mBuffer, mBufferLength);
	info += ", MachineIndex : ";
	info += StringUtility::intToString(mDataRidingHeartRate->mMachineIndex);
	info += ", HeartRate : ";
	info += StringUtility::intToString(mDataRidingHeartRate->getHeartRate());
	mLogSystem->log(info);
}