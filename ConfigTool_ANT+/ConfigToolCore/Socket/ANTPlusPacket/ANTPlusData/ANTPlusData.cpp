#include "ANTPlusData.h"
#include "ANTPlusPacket.h"
#include "txUtility.h"
#include "txSerializer.h"
#include "ToolCoreUtility.h"

ANTPlusData::ANTPlusData(ANTPlusPacket* packet)
{
	mParentPacket = packet;
}
ANTPlusData::~ANTPlusData()
{
	TRACE_DELETE_ARRAY(mBuffer);
}
void ANTPlusData::copyData(ANTPlusData* data)
{
	setData(data->mBuffer);
}
void ANTPlusData::setData(char* data)
{
	memcpy(mBuffer, data, mLength);
	parseData();
}
void ANTPlusData::setLength(int length)
{
	mLength = length;
	mBuffer = TRACE_NEW_ARRAY(char, mLength, mBuffer);
}