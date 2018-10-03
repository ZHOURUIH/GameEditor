#ifndef _MONSTER_ACTION_H_
#define _MONSTER_ACTION_H_

#include "ServerDefine.h"
#include "MonsterImage.h"
#include "ImageDefine.h"

// 一个动作的所有序列帧
class MonsterActionAnim
{
public:
	txVector<MonsterImage> mImageFrame;
public:
	void addFrame(const MonsterImage& frame)
	{
		// 只添加有效的序列帧
		if (frame.isValidImage())
		{
			mImageFrame.push_back(frame);
		}
	}
};

// 动作所有方向的序列帧
class MonsterActionSet
{
public:
	MonsterActionAnim mDirectionAction[DIRECTION_COUNT];
public:
	void addFrame(const MonsterImage& frame)
	{
		mDirectionAction[frame.mDirection].addFrame(frame);
	}
};

// 表示一套衣服的所有动作
class MonsterImageGroup
{
public:
	txMap<std::string, MonsterActionSet> mAllAction;
public:
	void addImage(const MonsterImage& image)
	{
		if (!mAllAction.contains(image.mActionName))
		{
			mAllAction.insert(image.mActionName, MonsterActionSet());
		}
		mAllAction[image.mActionName].addFrame(image);
	}
};

#endif