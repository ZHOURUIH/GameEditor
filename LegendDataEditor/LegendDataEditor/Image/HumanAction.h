#ifndef _HUMAN_ACTION_H_
#define _HUMAN_ACTION_H_

#include "ServerDefine.h"
#include "HumanImage.h"
#include "ImageDefine.h"

// 一个动作的所有序列帧
class HumanActionAnim
{
public:
	myVector<HumanImage> mImageFrame;
public:
	void addFrame(const HumanImage& frame)
	{
		mImageFrame.push_back(frame);
	}
};

// 动作所有方向的序列帧
class HumanActionSet
{
public:
	HumanActionAnim mDirectionAction[DIRECTION_COUNT];
public:
	void addFrame(const HumanImage& frame)
	{
		mDirectionAction[frame.mDirection].addFrame(frame);
	}
};

// 表示一套衣服的所有动作
class HumanImageGroup
{
public:
	myMap<string, HumanActionSet> mAllAction;
public:
	void addImage(const HumanImage& image)
	{
		if (!mAllAction.contains(image.mActionName))
		{
			mAllAction.insert(image.mActionName, HumanActionSet());
		}
		mAllAction[image.mActionName].addFrame(image);
	}
};

#endif