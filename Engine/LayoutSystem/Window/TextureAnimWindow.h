#ifndef _TEXTURE_ANIM_WINDOW_H_
#define _TEXTURE_ANIM_WINDOW_H_

#include "TextureWindow.h"

class TextureAnimWindow : public TextureWindow
{
public:
	TextureAnimWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~TextureAnimWindow(){}
	virtual void update(const float& elapsedTime);

	const LOOP_MODE& getLoop() const	{ return mLoopMode; }
	const std::string& getTextureSet()	{ return mTextureSet; }
	const float& getInterval()			{ return mInterval; }
	const int& getStartIndex()			{ return mStartIndex; }
	const PLAY_STATE& getPlayState()	{ return mPlayState; }
	const bool& getPlayDirection()		{ return mPlayDirection; }
	const int& getEndIndex()			{ return mEndIndex; }
	int getTextureFrameCount()			{ return (int)mTextureCache.size(); }
	const bool& getAutoHide()			{ return mAutoHide; }
	// 获得实际的终止下标,如果是自动获得,则返回最后一张的下标
	int getRealEndIndex()
	{
		if (mEndIndex < 0)
		{
			return txMath::getMax(getTextureFrameCount() - 1, 0);
		}
		else
		{
			return mEndIndex;
		}
	}

	void setLoop(const LOOP_MODE& loop)				{ mLoopMode = loop; }
	void setInterval(const float& interval)			{ mInterval = interval; }
	void setPlayDirection(const bool& direction)	{ mPlayDirection = direction; }
	void setAutoHide(const bool& autoHide)			{ mAutoHide = autoHide; }
	void setPlayState(const PLAY_STATE& state);
	void setStartIndex(const int& startIndex)		
	{
		mStartIndex = startIndex;
		txMath::clamp(mStartIndex, 0, getTextureFrameCount() - 1);
	}
	void setEndIndex(const int& endIndex)			
	{ 
		mEndIndex = endIndex;
		if (mEndIndex >= 0)
		{
			txMath::clamp(mEndIndex, 0, getTextureFrameCount() - 1);
		}
	}
	void setTextureSet(const std::string& textureSetName, const bool& async);	
	void stop(const bool& resetStartIndex = true, const bool& callback = true, const bool& isBreak = true)
	{
		mPlayState = PS_STOP;
		if (resetStartIndex)
		{
			setCurFrameIndex(mStartIndex);
		}
		// 中断序列帧播放时调用回调函数,只在非循环播放时才调用
		if (callback && mLoopMode == LM_NONE)
		{
			callAndClearEndCallback(isBreak);
		}
	}
		
	void play() { mPlayState = PS_PLAY; }
	void pause() { mPlayState = PS_PAUSE; }
	void setPlayEndCallback(TextureAnimPlayEndCallback callback, void* userData)
	{
		callAndClearEndCallback(true);
		mPlayEndCallback = callback;
		mPlayEndUserData = userData;
	}
	void setCurFrameIndex(const int& index)
	{
		mCurTextureIndex = index;
		// 限制在起始帧和终止帧之间
		txMath::clamp(mCurTextureIndex, mStartIndex, getRealEndIndex());
		if (mCurTextureIndex >= 0 && mCurTextureIndex < (int)mTextureCache.size())
		{
			mTexture = mTextureCache[mCurTextureIndex];
		}
		mCurTimeCount = 0.0f;
	}
	virtual void finishAsycLoad();
	virtual void getUsedResourceList(txVector<std::string>& resourceList);
	static const txSet<std::string>& getTextureAnimNameList() { return mTextureAnimNameList; }
	static void refreshTextureAnimNameList();
protected:
	virtual void initProperty();
	// 调用并且清空回调,清空是在调用之前
	void callAndClearEndCallback(bool isBreak)
	{
		TextureAnimPlayEndCallback curCallback = mPlayEndCallback;
		void* curUserData = mPlayEndUserData;
		mPlayEndCallback = NULL;
		mPlayEndUserData = NULL;
		// 如果回调函数当前不为空,则是中断了更新
		if (curCallback != NULL)
		{
			curCallback(this, curUserData, isBreak);
		}
	}
protected:
	txVector<txTexture*> mTextureCache;
	int mStartIndex;		// 序列帧的起始帧下标,默认为0,即从头开始
	int mEndIndex;			// 序列帧的终止帧下标,默认为-1,即播放到尾部
	bool mPlayDirection;	// 播放方向,true为正向播放(从mStartIndex到mEndIndex),false为返向播放(从mEndIndex到mStartIndex)
	int mCurTextureIndex;
	LOOP_MODE mLoopMode;
	std::string mTextureSet;
	float mCurTimeCount;
	PLAY_STATE mPlayState;
	float mInterval;	// 隔多少秒切换图片
	bool mAutoHide;		// 是否在播放完毕后自动隐藏并且重置当前帧下标
	TextureAnimPlayEndCallback mPlayEndCallback;	// 一个序列播放完时的回调函数,只在非循环播放状态下有效
	void* mPlayEndUserData;
	static txSet<std::string> mTextureAnimNameList;
};

#endif