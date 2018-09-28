#ifndef _HUMAN_ACTION_H_
#define _HUMAN_ACTION_H_

#include "ServerDefine.h"
#include "HumanImage.h"
#include "ImageDefine.h"

// 一个动作的所有序列帧
class ActionAnim
{
public:
	txVector<HumanImage> mHumanImageFrame;
public:
	void addFrame(const HumanImage& frame)
	{
		// 只添加有效的序列帧
		if (frame.isValidImage())
		{
			mHumanImageFrame.push_back(frame);
		}
	}
};

// 动作所有方向的序列帧
class ActionSet
{
public:
	ActionAnim mDirectionAction[DIRECTION_COUNT];
public:
	void addFrame(const HumanImage& frame)
	{
		mDirectionAction[frame.mDirection].addFrame(frame);
	}
};

// 表示一套衣服的所有动作
class ImageGroup
{
public:
	txMap<std::string, ActionSet> mAllAction;
public:
	void addImage(const HumanImage& image)
	{
		if (!mAllAction.contains(image.mActionName))
		{
			mAllAction.insert(image.mActionName, ActionSet());
		}
		mAllAction[image.mActionName].addFrame(image);
	}
};

#endif