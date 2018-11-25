#ifndef _NPC_ACTION_H_
#define _NPC_ACTION_H_

#include "ServerDefine.h"
#include "NPCImage.h"
#include "ImageDefine.h"

// 一个动作的所有序列帧
class NPCActionAnim
{
public:
	txVector<NPCImage> mImageFrame;
public:
	void addFrame(const NPCImage& frame)
	{
		// 只添加有效的序列帧
		if (frame.isValidImage())
		{
			mImageFrame.push_back(frame);
		}
	}
};

// 动作所有方向的序列帧
class NPCActionSet
{
public:
	NPCActionAnim mDirectionAction[DIRECTION_COUNT];
public:
	void addFrame(const NPCImage& frame)
	{
		mDirectionAction[frame.mDirection].addFrame(frame);
	}
};

// 表示一套衣服的所有动作
class NPCImageGroup
{
public:
	txMap<string, NPCActionSet> mAllAction;
public:
	void addImage(const NPCImage& image)
	{
		if (!mAllAction.contains(image.mActionName))
		{
			mAllAction.insert(image.mActionName, NPCActionSet());
		}
		mAllAction[image.mActionName].addFrame(image);
	}
};

#endif