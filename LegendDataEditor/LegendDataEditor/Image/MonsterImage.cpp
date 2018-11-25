#include "MonsterImage.h"
#include "Utility.h"

void MonsterImage::setFileName(const string& fileName)
{
	txVector<string> elemList;
	StringUtility::split(fileName, "_", elemList);
	if (elemList.size() != 3)
	{
		std::cout << "monster file name error : " << fileName << std::endl;
		return;
	}
	mActionName = elemList[0];
	mDirection = StringUtility::getLastNumber(elemList[1]);
	mFrameIndex = StringUtility::stringToInt(elemList[2]);
	mActionIndex = -1;
	for (int i = 0; i < MONSTER_ACTION_COUNT; ++i)
	{
		if (MONSTER_ACTION[i].mName == mActionName)
		{
			mActionIndex = i;
			break;
		}
	}
}