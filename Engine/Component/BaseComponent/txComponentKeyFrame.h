#ifndef _TX_COMPONENT_KEY_FRAME_H_
#define _TX_COMPONENT_KEY_FRAME_H_

#include "txComponent.h"

class txTrembling;
class txComponentKeyFrame : public txComponent
{
public:
	txComponentKeyFrame(const std::string& type, const std::string& name)
	:
	txComponent(type, name)
	{
		mKeyFrame = NULL;
		mOnceLength = 1.0f;
		mTremblingName = EMPTY_STRING;
		mLoop = false;
		mAmplitude = 1.0f;
		mCurrentTime = 0.0f;
		mPlayLength = 0.0f;
		mPlayedTime = 0.0f;
		mPlayState = PS_STOP;
		mOffset = 0.0f;
		mFullOnce = false;
		clearCallback();
	}
	virtual void initProperty();
	virtual void update(const float& elapsedTime);
	virtual  void setActive(const bool& active)
	{
		txComponent::setActive(active);
		if (!active)
		{
			stop();
		}
	}
	virtual void play(const std::string& name, const bool& loop = false, const float& onceLength = 1.0f,
		const float& offset = 0.0f, const bool& fullOnce = true, const float& amplitude = 1.0f);
	virtual void stop(const bool& force = false);
	virtual void pause() { mPlayState = PS_PAUSE; }
	void setState(const PLAY_STATE& state);
	float getTremblingPercent()
	{
		return mOnceLength > 0.0f ? mCurrentTime / mOnceLength : 0.0f;
	}
	void setDoingCallback(KeyFrameCallback callback, void* userData)
	{
		setCallback(callback, userData, mDoingCallback, mDoingUserData, this);
	}
	void setDoneCallback(KeyFrameCallback callback, void* userData)
	{
		setCallback(callback, userData, mDoneCallback, mDoneUserData, this);
	}
	static void setCallback(KeyFrameCallback callback, void* userData, KeyFrameCallback& curCallback, void*& curUserData, txComponentKeyFrame* component);
	// 获得成员变量
	const bool& getLoop()					{ return mLoop; }
	const float& getOnceLength()			{ return mOnceLength; }
	const float& getAmplitude()				{ return mAmplitude; }
	const float& getOffset()				{ return mOffset; }
	const bool& getFullOnce()				{ return mFullOnce; }
	const PLAY_STATE& getState()			{ return mPlayState; }
	const float& getCurrentTime()			{ return mCurrentTime; }
	txTrembling* getKeyFrame()				{ return mKeyFrame; }
	const std::string& getTremblingName()	{ return mTremblingName; }

	// 设置成员变量
	void setLoop(const bool& loop)				{ mLoop = loop; }
	void setOnceLength(const float& length)		{ mOnceLength = length; }
	void setAmplitude(const float& amplitude)	{ mAmplitude = amplitude; }
	void setOffset(const float& offset)			{ mOffset = offset; }
	void setFullOnce(const bool& fullOnce)		{ mFullOnce = fullOnce; }
	void setCurrentTime(const float& time)		{ mCurrentTime = time; }
	void setTrembling(const std::string& name)	{ mTremblingName = name; }
	//----------------------------------------------------------------------------------------------------------------------------
protected:
	void clearCallback()
	{
		mDoingCallback = NULL;
		mDoingUserData = NULL;
		mDoneCallback = NULL;
		mDoneUserData = NULL;
	}
	void afterApllyTrembling(const bool& done)
	{
		if (mDoingCallback != NULL)
		{
			mDoingCallback(this, mDoingUserData, false, done);
		}

		if (done)
		{
			setActive(false);
			// 强制停止组件
			stop(true);
			doneCallback(mDoneCallback, mDoneUserData, this);
		}
	}
	static void doneCallback(KeyFrameCallback& curDoneCallback, void*& curDoneUserData, txComponentKeyFrame* component)
	{
		// 先保存回调,然后再调用回调之前就清空回调,确保在回调函数执行时已经完全完成
		KeyFrameCallback tempCallback = curDoneCallback;
		void* tempUserData = curDoneUserData;
		component->clearCallback();
		if (tempCallback != NULL)
		{
			tempCallback(component, tempUserData, false, true);
		}
	}
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentKeyFrame); }
	virtual void setBaseType() { mBaseType = TOSTRING(txComponentKeyFrame); }
	virtual void applyKeyFrame(const txVector<float>& value) = 0;
protected:
	txTrembling*	mKeyFrame;		// 当前使用的关键帧
	float			mOnceLength;		// 关键帧长度默认为1秒
	std::string		mTremblingName;
	bool			mLoop;
	float			mAmplitude;
	float			mCurrentTime;		// 从上一次从头开始播放到现在的时长
	float			mPlayLength;		// 小于0则无限播放, 大于0则播放length时长
	float			mPlayedTime;		// 本次震动已经播放的时长,从上一次开始播放到现在的累计时长
	PLAY_STATE		mPlayState;
	float			mOffset;
	bool			mFullOnce;
	txVector<float> mStopValue;		// 当组件停止时,需要应用的关键帧值
	KeyFrameCallback mDoingCallback;
	void*			mDoingUserData;
	KeyFrameCallback mDoneCallback;
	void*			mDoneUserData;
	txVector<float> mCurValueList;		// 仅用作在更新中获取当前时间点的值,避免重复开辟内存
};

#endif