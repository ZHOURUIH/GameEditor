#include "ANTPlusDataRidingCadence.h"
#include "ANTPlusPacket.h"
#include "txUtility.h"
#include "txSerializer.h"
#include "ToolCoreUtility.h"

ANTPlusDataRidingCadence::ANTPlusDataRidingCadence(ANTPlusPacket* packet)
:ANTPlusDataRiding(packet)
{
	;
}

int ANTPlusDataRidingCadence::getCadence()
{
	if (mParentPacket->mLastPacket == NULL)
	{
		return 0;
	}
	ANTPlusDataRiding* lastData = static_cast<ANTPlusDataRiding*>(mParentPacket->mLastPacket->mData);
	int index = 4;
	unsigned short lastTime = BinaryUtility::read(lastTime, lastData->mPageData, index);
	unsigned short lastCount = BinaryUtility::read(lastCount, lastData->mPageData, index);
	index = 4;
	unsigned short curTime = BinaryUtility::read(curTime, mPageData, index);
	unsigned short curCount = BinaryUtility::read(curCount, mPageData, index);
	unsigned short timeDelta = curTime - lastTime;
	unsigned short countDelta = curCount - lastCount;
	if (timeDelta != 0)
	{
		int cadence = (int)(countDelta / (timeDelta / 1024.0f) * 60.0f);
		return cadence;
	}
	else
	{
		return 0;
	}
}

bool ANTPlusDataRidingCadence::isSameDataValue(ANTPlusDataRiding* newData)
{
	return BinaryUtility::isMemoryEqual((char*)newData->mPageData + 4, (char*)mPageData + 4, ANTPlusDataRiding::PAGE_DATA_LENGTH - 4);
}