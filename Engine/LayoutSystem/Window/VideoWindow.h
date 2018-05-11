#ifndef _VIDEO_WINDOW_H_
#define _VIDEO_WINDOW_H_

#include "TextureWindow.h"

class txVideoPlayer;
class txVideoManager;
class txTextureManager;
class VideoWindow : public TextureWindow
{
public:
	VideoWindow(const std::string& name, const WINDOW_TYPE& type);
	virtual ~VideoWindow();
	virtual void update(const float& elapsedTime);
	void destroy();
	void setRate(const float& rate) { mRate = rate; }
	void setPlayPercent(const float& percent){}
	void setPlayTime(const float& fTime) {}
	void setPlayState(const PLAY_STATE& state, const bool& callback = true);	// callback是在停止视频时是否调用回调函数
	void setFileName(const std::string& fileName){ mFileName = fileName; setPlayState(PS_STOP); }
	void setLoop(const bool& loop){ mLoop = loop; }

	const float& getRate() { return mRate; }
	float getPlayPercent() { return 0.0f; }
	const float& getPlayTime() { return mVideoTime; }
	const PLAY_STATE& getPlayState() { return mVideoState; }
	const std::string& getFileName(){ return mFileName; }
	const bool& getLoop(){ return mLoop; }

	float getVideoLength();
	float getVideoFPS();
	VECTOR2 getVideoSize();
	void setVideoPlayEndCallback(VideoPlayEndCallback endCallback, void* userData)
	{
		callAndClearEndCallback(true);
		mEndCallback = endCallback;
		mEndUserData = userData;
	}
	void preDecode();
protected:
	virtual void initProperty();
	void createPlayer();
	void destroyPlayer();
	void callAndClearEndCallback(const bool& isBreak)
	{
		VideoPlayEndCallback curCallback = mEndCallback;
		void* curUserData = mEndUserData;
		mEndCallback = NULL;
		mEndUserData = NULL;
		// 如果回调函数当前不为空,则是中断了更新
		if (curCallback != NULL)
		{
			curCallback(this, curUserData, isBreak);
		}
	}
protected:
	txVideoPlayer* mVideoPlayer;
	std::string mFileName;
	int mCurFrame;			// 当前帧,-1表示该帧已经渲染了,否则表示还没有渲染
	int mLastFrame;
	float mVideoTime;		// 视频实际播放到的时间
	float mRate;
	PLAY_STATE mVideoState;
	bool mLoop;
	VideoPlayEndCallback mEndCallback;
	void* mEndUserData;
};

#endif