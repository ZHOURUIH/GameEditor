#include "EffectImage.h"

void EffectImage::setFileName(const string& fileName)
{
	myVector<string> elemList;
	split(fileName.c_str(), "_", elemList);
	if (elemList.size() == 3)
	{
		mActionName = elemList[0];
		mDirection = getLastNumber(elemList[1]);
		mFrameIndex = stringToInt(elemList[2]);
	}
	else
	{
		cout << "effect file name error : " << fileName << endl;
		return;
	}
}