#ifndef _WEAPON_IMAGE_H_
#define _WEAPON_IMAGE_H_

#include "ServerDefine.h"
#include "ImageDefine.h"

class WeaponImage
{
public:
	string mLabel;
	int mPosX;
	int mPosY;
	string mActionName;
	int mDirection;
	int mFrameIndex;		// 在当前动作中的下标
	int mWeaponID;
	int mActionIndex;		// 动作在数组中的下标
public:
	void setFileName(const string& fileName);
	bool isValidImage() const { return mFrameIndex < HUMAN_ACTION[mActionIndex].mFrameCount; }
};

#endif