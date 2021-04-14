#include "HumanImage.h"

void HumanImage::setFileName(const string& fileName)
{
	myVector<string> elemList;
	split(fileName.c_str(), "_", elemList);
	if (elemList.size() != 3)
	{
		cout << "human file name error : " << fileName << endl;
		return;
	}
	mActionName = elemList[0];
	mDirection = getLastNumber(elemList[1]);
	mFrameIndex = stringToInt(elemList[2]);
	mActionIndex = -1;
	for (int i = 0; i < HUMAN_ACTION_COUNT; ++i)
	{
		if (HUMAN_ACTION[i].mName == mActionName)
		{
			mActionIndex = i;
			break;
		}
	}
}