#ifndef _HUMAN_IMAGE_H_
#define _HUMAN_IMAGE_H_

#include "ServerBase.h"
#include "ImageDefine.h"

class HumanImage : public ServerBase
{
public:
	string mLabel;
	int mPosX;
	int mPosY;
	string mActionName;
	int mDirection;
	int mFrameIndex;		// 在当前动作中的下标
	int mClothID;
	int mActionIndex;		// 动作在数组中的下标
public:
	void setFileName(const string& fileName);
};

#endif