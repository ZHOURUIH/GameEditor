#include "ANTPlusDataHeartBeat.h"
#include "ANTPlusPacket.h"
#include "txUtility.h"
#include "txSerializer.h"
#include "ToolCoreUtility.h"

ANTPlusDataHeartBeat::ANTPlusDataHeartBeat(ANTPlusPacket* packet)
:ANTPlusData(packet)
{
	mCmdID = 0x02;
	setLength(3);
}
void ANTPlusDataHeartBeat::parseData()
{
	txSerializer serializer(mBuffer, mLength);
	serializer.read(mBattery);
	serializer.read(mReserve);
}