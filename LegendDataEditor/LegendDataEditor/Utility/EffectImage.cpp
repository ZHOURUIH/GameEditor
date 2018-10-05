#include "EffectImage.h"
#include "Utility.h"

void EffectImage::setFileName(const std::string& fileName)
{
	txVector<std::string> elemList;
	StringUtility::split(fileName, "_", elemList);
	if (elemList.size() > 2)
	{
		int nameElemCount = 0;
		// 倒数第二个是方向信息的,计算方向,方向前面是特效名
		if (StringUtility::getNotNumberSubString(elemList[elemList.size() - 2]) == "dir")
		{	
			nameElemCount = elemList.size() - 2;
			mDirection = StringUtility::getLastNumber(elemList[elemList.size() - 2]);
		}
		// 不包含方向信息,下标前都是特效名
		else
		{
			nameElemCount = elemList.size() - 1;
			mDirection = 0;
		}
		mEffectName = "";
		for (int i = 0; i < nameElemCount; ++i)
		{
			mEffectName += elemList[0] + "_";
		}
		StringUtility::removeLast(mEffectName, "_");
		mFrameIndex = StringUtility::stringToInt(elemList[elemList.size() - 1]);
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