#include "ANTPlusDataRiding.h"
#include "ANTPlusPacket.h"
#include "txUtility.h"
#include "txSerializer.h"
#include "ToolCoreUtility.h"

ANTPlusDataRiding::ANTPlusDataRiding(ANTPlusPacket* packet)
	:ANTPlusData(packet)
{
	mCmdID = 0x01;
	setLength(16);
	mMachineIndex = -1;
}
void ANTPlusDataRiding::copyData(ANTPlusData* data)
{
	ANTPlusData::copyData(data);
	mMachineIndex = static_cast<ANTPlusDataRiding*>(data)->mMachineIndex;
}
void ANTPlusDataRiding::parseData()
{
	txSerializer serializer(mBuffer, mLength);
	serializer.read(mDeviceType);
	serializer.readBuffer((char*)mDeviceID, DEVICE_ID_LENGTH, DEVICE_ID_LENGTH);
	serializer.readBuffer((char*)mPageData, PAGE_DATA_LENGTH, PAGE_DATA_LENGTH);
	serializer.read(mRSSIValue);
	serializer.read(mReserve);
	if (mDeviceType >= RD_HEART_RATE && mDeviceType <= RD_SPEED)
	{
		mRidingDevice = (RIDING_DEVICE)mDeviceType;
	}
	else
	{
		mRidingDevice = RD_NONE;
	}
}