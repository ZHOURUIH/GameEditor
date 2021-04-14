#include "MonsterImage.h"

void MonsterImage::setFileName(const string& fileName)
{
	myVector<string> elemList;
	split(fileName.c_str(), "_", elemList);
	if (elemList.size() != 3)
	{
		std::cout << "monster file name error : " << fileName << std::endl;
		return;
	}
	mActionName = elemList[0];
	mDirection = getLastNumber(elemList[1]);
	mFrameIndex = stringToInt(elemList[2]);
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