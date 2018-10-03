#ifndef _WEAPON_ACTION_H_
#define _WEAPON_ACTION_H_

#include "ServerDefine.h"
#include "WeaponImage.h"
#include "ImageDefine.h"

// 一个动作的所有序列帧
class WeaponActionAnim
{
public:
	txVector<WeaponImage> mImageFrame;
public:
	void addFrame(const WeaponImage& frame)
	{
		// 只添加有效的序列帧
		if (frame.isValidImage())
		{
			mImageFrame.push_back(frame);
		}
	}
};

// 动作所有方向的序列帧
class WeaponActionSet
{
public:
	WeaponActionAnim mDirectionAction[DIRECTION_COUNT];
public:
	void addFrame(const WeaponImage& frame)
	{
		mDirectionAction[frame.mDirection].addFrame(frame);
	}
};

// 表示一套衣服的所有动作
class WeaponImageGroup
{
public:
	txMap<std::string, WeaponActionSet> mAllAction;
public:
	void addImage(const WeaponImage& image)
	{
		if (!mAllAction.contains(image.mActionName))
		{
			mAllAction.insert(image.mActionName, WeaponActionSet());
		}
		mAllAction[image.mActionName].addFrame(image);
	}
};

#endif