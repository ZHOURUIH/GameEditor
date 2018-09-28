#ifndef _HUMAN_IMAGE_H_
#define _HUMAN_IMAGE_H_

#include "ServerDefine.h"
#include "ImageDefine.h"

class HumanImage
{
public:
	int mPosX;
	int mPosY;
	std::string mOriginFileName;
	int mIndexInGroup;		// 在当前外观中的下标
	std::string mActionName;
	int mDirection;
	int mFrameIndex;		// 在当前动作中的下标
	int mClothID;
	int mActionIndex;		// 动作在数组中的下标
public:
	void moveImage(const std::string& prePath);
	void setIndexInGroup(int index);
	bool isValidImage() const { return mFrameIndex < Action[mActionIndex].mFrameCount; }
};

#endif