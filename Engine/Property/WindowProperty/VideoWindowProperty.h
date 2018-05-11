#ifndef _VIDEO_WINDOW_PROPERTY_H_
#define _VIDEO_WINDOW_PROPERTY_H_

#include "WindowProperty.h"
	
WINDOW_PROPERTY(VideoWindow)

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频文件名
class VideoWindowPropertyVideo : public VideoWindowProperty<txStringProperty, std::string>
{
public:
	VideoWindowPropertyVideo();
	virtual void setRealValue(const std::string& value, VideoWindow* window);
	virtual std::string getRealValue(VideoWindow* window);
	virtual txVector<std::string>* getChoices() { return &mChoices; }
	static void setChoices(const txVector<std::string>& choices) { mChoices = choices; }
protected:
	static txVector<std::string> mChoices;
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频播放状态
class VideoWindowPropertyPlayState : public VideoWindowProperty<txEnumProperty<PLAY_STATE>, PLAY_STATE>
{
public:
	VideoWindowPropertyPlayState();
	virtual void setRealValue(const PLAY_STATE& value, VideoWindow* window);
	virtual PLAY_STATE getRealValue(VideoWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频循环播放
class VideoWindowPropertyVideoLoop : public VideoWindowProperty<txBoolProperty, bool>
{
public:
	VideoWindowPropertyVideoLoop();
	virtual void setRealValue(const bool& value, VideoWindow* window);
	virtual bool getRealValue(VideoWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频播放速率
class VideoWindowPropertyRate : public VideoWindowProperty<txFloatProperty, float>
{
public:
	VideoWindowPropertyRate();
	virtual void setRealValue(const float& value, VideoWindow* window);
	virtual float getRealValue(VideoWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频长度
class VideoWindowPropertyVideoLength : public VideoWindowProperty<txFloatProperty, float>
{
public:
	VideoWindowPropertyVideoLength();
	virtual void setRealValue(const float& value, VideoWindow* window);
	virtual float getRealValue(VideoWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频当前播放的时间
class VideoWindowPropertyPlayTime : public VideoWindowProperty<txFloatProperty, float>
{
public:
	VideoWindowPropertyPlayTime();
	virtual void setRealValue(const float& value, VideoWindow* window);
	virtual float getRealValue(VideoWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频当前播放的百分比
class VideoWindowPropertyPlayPercent : public VideoWindowProperty<txFloatProperty, float>
{
public:
	VideoWindowPropertyPlayPercent();
	virtual void setRealValue(const float& value, VideoWindow* window);
	virtual float getRealValue(VideoWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频的分辨率
class VideoWindowPropertyVideoSize : public VideoWindowProperty<txVector2Property, VECTOR2>
{
public:
	VideoWindowPropertyVideoSize();
	virtual void setRealValue(const VECTOR2& value, VideoWindow* window);
	virtual VECTOR2 getRealValue(VideoWindow* window);
};

//---------------------------------------------------------------------------------------------------------------------------------------------
// 视频的帧率
class VideoWindowPropertyVideoFPS : public VideoWindowProperty<txFloatProperty, float>
{
public:
	VideoWindowPropertyVideoFPS();
	virtual void setRealValue(const float& value, VideoWindow* window);
	virtual float getRealValue(VideoWindow* window);
};

#endif