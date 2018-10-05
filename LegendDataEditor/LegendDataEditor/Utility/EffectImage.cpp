#include "EffectImage.h"
#include "Utility.h"

void EffectImage::setFileName(const std::string& fileName)
{
	txVector<std::string> elemList;
	StringUtility::split(fileName, "_", elemList);
	if (elemList.size() == 3)
	{
		// 包含方向信息的,计算方向
		if (StringUtility::getNotNumberSubString(elemList[1]) == "dir")
		{
			mEffectName = elemList[0];
			mDirection = StringUtility::getLastNumber(elemList[1]);
			mFrameIndex = StringUtility::stringToInt(elemList[2]);
		}
		// 不包含方向信息的,前两部分作为特效名
		else
		{
			mEffectName = elemList[0] + "_" + elemList[1];
			mDirection = 0;
			mFrameIndex = StringUtility::stringToInt(elemList[2]);
		}
	}
	else if (elemList.size() == 2)
	{
		mEffectName = elemList[0];
		mDirection = 0;
		mFrameIndex = StringUtility::stringToInt(elemList[1]);
	}
	else
	{
		std::cout << "effect file name error : " << fileName << std::endl;
		return;
	}
}