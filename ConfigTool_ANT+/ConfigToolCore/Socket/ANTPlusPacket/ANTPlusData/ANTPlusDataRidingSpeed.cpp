#include "ANTPlusDataRidingSpeed.h"
#include "ANTPlusPacket.h"
#include "txUtility.h"
#include "txSerializer.h"
#include "ToolCoreUtility.h"

ANTPlusDataRidingSpeed::ANTPlusDataRidingSpeed(ANTPlusPacket* packet)
:ANTPlusDataRiding(packet)
{
	;
}

float ANTPlusDataRidingSpeed::getSpeed(float perimeter)
{
	if (mParentPacket->mLastPacket == NULL)
	{
		return 0.0f;
	}
	ANTPlusDataRiding* lastData = static_cast<ANTPlusDataRiding*>(mParentPacket->mLastPacket->mData);
	int index = 4;
	unsigned short lastTime = BinaryUtility::read(lastTime, lastData->mPageData, index);
	unsigned short lastCircle = BinaryUtility::read(lastCircle, lastData->mPageData, index);
	index = 4;
	unsigned short curTime = BinaryUtility::read(curTime, mPageData, index);
	unsigned short curCircle = BinaryUtility::read(curCircle, mPageData, index);
	unsigned short timeDelta = curTime - lastTime;
	unsigned short circleDelta = curCircle - lastCircle;
	if (timeDelta != 0)
	{
		return circleDelta * perimeter / (timeDelta / 1024.0f);
	}
	else
	{
		return 0.0f;
	}
}

bool ANTPlusDataRidingSpeed::isSameDataValue(ANTPlusDataRiding* newData)
{
	return BinaryUtility::isMemoryEqual((char*)newData->mPageData + 4, (char*)mPageData + 4, ANTPlusDataRiding::PAGE_DATA_LENGTH - 4);
}