#ifndef _EFFECT_IMAGE_H_
#define _EFFECT_IMAGE_H_

#include "ServerBase.h"
#include "ImageDefine.h"

// 一个特效的一帧
class EffectImage : public ServerBase
{
public:
	string mLabel;
	int mPosX;
	int mPosY;
	int mDirection;
	string mActionName;
	int mFrameIndex;		// 在当前序列帧中的下标
	int mID;
public:
	void setFileName(const string& fileName);
};

#endif