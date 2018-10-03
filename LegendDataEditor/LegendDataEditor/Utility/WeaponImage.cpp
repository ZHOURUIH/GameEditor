#include "WeaponImage.h"
#include "Utility.h"

void WeaponImage::setFileName(const std::string& fileName)
{
	txVector<std::string> elemList;
	StringUtility::split(fileName, "_", elemList);
	if (elemList.size() != 3)
	{
		std::cout << "human file name error : " << fileName << std::endl;
		return;
	}
	mActionName = elemList[0];
	mDirection = StringUtility::getLastNumber(elemList[1]);
	mFrameIndex = StringUtility::stringToInt(elemList[2]);
	mActionIndex = -1;
	for (int i = 0; i < MONSTER_ACTION_COUNT; ++i)
	{
		if (HUMAN_ACTION[i].mName == mActionName)
		{
			mActionIndex = i;
			break;
		}
	}
}