#include "ANTPlusDataRidingHeartRate.h"
#include "ANTPlusPacket.h"
#include "txUtility.h"
#include "txSerializer.h"
#include "ToolCoreUtility.h"

ANTPlusDataRidingHeartRate::ANTPlusDataRidingHeartRate(ANTPlusPacket* packet)
:ANTPlusDataRiding(packet)
{
	;
}
int ANTPlusDataRidingHeartRate::getHeartRate()
{
	return mPageData[PAGE_DATA_LENGTH - 1];
}