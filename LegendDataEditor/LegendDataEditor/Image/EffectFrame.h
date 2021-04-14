#ifndef _EFFECT_FRAME_H_
#define _EFFECT_FRAME_H_

#include "ServerDefine.h"
#include "ImageDefine.h"
#include "EffectImage.h"

// 一个特效的所有序列帧
class EffectAnim
{
public:
	myVector<EffectImage> mImageFrame;
public:
	void addFrame(const EffectImage& frame)
	{
		mImageFrame.push_back(frame);
	}
};

// 特效所有方向的序列帧
class EffectSet
{
public:
	myMap<int, EffectAnim> mDirectionAction;
public:
	void addFrame(const EffectImage& frame)
	{
		if (!mDirectionAction.contains(frame.mDirection))
		{
			mDirectionAction.insert(frame.mDirection, EffectAnim());
		}
		mDirectionAction[frame.mDirection].addFrame(frame);
	}
};

// 表示一套特效的所有动作
class EffectImageGroup
{
public:
	myMap<string, EffectSet> mAllEffect;
public:
	void addImage(const EffectImage& image)
	{
		if (!mAllEffect.contains(image.mActionName))
		{
			mAllEffect.insert(image.mActionName, EffectSet());
		}
		mAllEffect[image.mActionName].addFrame(image);
	}
};

#endif