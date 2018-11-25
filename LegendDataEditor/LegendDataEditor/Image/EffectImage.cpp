#include "EffectImage.h"
#include "Utility.h"

void EffectImage::setFileName(const string& fileName)
{
	txVector<string> elemList;
	StringUtility::split(fileName, "_", elemList);
	if (elemList.size() == 3)
	{
		mActionName = elemList[0];
		mDirection = StringUtility::getLastNumber(elemList[1]);
		mFrameIndex = StringUtility::stringToInt(elemList[2]);
	}
	else
	{
		std::cout << "effect file name error : " << fileName << std::endl;
		return;
	}
}