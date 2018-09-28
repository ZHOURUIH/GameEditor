#include "HumanImage.h"
#include "Utility.h"

void HumanImage::moveImage(const std::string& prePath)
{
	// 只移动有效图片
	if (isValidImage())
	{
		std::string animSetName = mActionName + "_dir" + StringUtility::intToString(mDirection);
		std::string fileName = animSetName + "_" + StringUtility::intToString(mFrameIndex) + ".png";
		std::string path = prePath + "/human_cloth" + StringUtility::intToString(mClothID) + "_" + animSetName + "/";
		FileUtility::moveFile(mOriginFileName, path + fileName);
	}
}

void HumanImage::setIndexInGroup(int index)
{
	mIndexInGroup = index;
	int i = 0;
	while (true)
	{
		if (index - Action[i].mMaxFrame * DIRECTION_COUNT < 0)
		{
			break;
		}
		index -= Action[i].mMaxFrame * DIRECTION_COUNT;
		++i;
	}
	mActionIndex = i;
	// 因为一组动作资源包含了8个方向上的所有动作,所以可以根据下标计算出方向和序列帧下标
	mDirection = index / Action[mActionIndex].mMaxFrame;
	mFrameIndex = index % Action[mActionIndex].mMaxFrame;
	mActionName = Action[mActionIndex].mName;
}