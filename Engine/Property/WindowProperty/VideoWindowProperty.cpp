#include "VideoWindowProperty.h"

//---------------------------------------------------------------------------------------
txVector<std::string> VideoWindowPropertyVideo::mChoices;
VideoWindowPropertyVideo::VideoWindowPropertyVideo()
{
	mPropertyName = "FileNameVideo";
	mDescription = "视频文件名";
}
void VideoWindowPropertyVideo::setRealValue(const std::string& value, VideoWindow* window)
{
	window->setFileName(value);
}
std::string VideoWindowPropertyVideo::getRealValue(VideoWindow* window)
{
	return window->getFileName();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyPlayState::VideoWindowPropertyPlayState()
{
	mPropertyName = "PlayState";
	mDescription = "播放状态";
	ADD_ENUM(PS_PLAY);
	ADD_ENUM(PS_PAUSE);
	ADD_ENUM(PS_STOP);
}
void VideoWindowPropertyPlayState::setRealValue(const PLAY_STATE& value, VideoWindow* window)
{
	window->setPlayState(value);
}
PLAY_STATE VideoWindowPropertyPlayState::getRealValue(VideoWindow* window)
{
	return window->getPlayState();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyVideoLoop::VideoWindowPropertyVideoLoop()
{
	mPropertyName = "VideoLoop";
	mDescription = "视频循环播放";
}
void VideoWindowPropertyVideoLoop::setRealValue(const bool& value, VideoWindow* window)
{
	window->setLoop(value);
}
bool VideoWindowPropertyVideoLoop::getRealValue(VideoWindow* window)
{
	return window->getLoop();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyRate::VideoWindowPropertyRate()
{
	mPropertyName = "Rate";
	mDescription = "视频播放速率";
}
void VideoWindowPropertyRate::setRealValue(const float& value, VideoWindow* window)
{
	window->setRate(value);
}
float VideoWindowPropertyRate::getRealValue(VideoWindow* window)
{
	return window->getRate();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyVideoLength::VideoWindowPropertyVideoLength()
{
	mPropertyName = "VideoLength";
	mDescription = "视频长度";
	mReadOnly = true;
}
void VideoWindowPropertyVideoLength::setRealValue(const float& value, VideoWindow* window)
{
	// 不实现设置视频长度
}
float VideoWindowPropertyVideoLength::getRealValue(VideoWindow* window)
{
	return window->getVideoLength();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyPlayTime::VideoWindowPropertyPlayTime()
{
	mPropertyName = "PlayTime";
	mDescription = "视频当前播放的时间";
}
void VideoWindowPropertyPlayTime::setRealValue(const float& value, VideoWindow* window)
{
	window->setPlayTime(value);
}
float VideoWindowPropertyPlayTime::getRealValue(VideoWindow* window)
{
	return window->getPlayTime();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyPlayPercent::VideoWindowPropertyPlayPercent()
{
	mPropertyName = "PlayPercent";
	mDescription = "视频当前播放的百分比";
}
void VideoWindowPropertyPlayPercent::setRealValue(const float& value, VideoWindow* window)
{
	window->setPlayPercent(value);
}
float VideoWindowPropertyPlayPercent::getRealValue(VideoWindow* window)
{
	return window->getPlayPercent();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyVideoSize::VideoWindowPropertyVideoSize()
{
	mPropertyName = "VideoSize";
	mDescription = "视频分辨率";
	mReadOnly = true;
}
void VideoWindowPropertyVideoSize::setRealValue(const VECTOR2& value, VideoWindow* window)
{
	// 不实现设置分辨率
}
VECTOR2 VideoWindowPropertyVideoSize::getRealValue(VideoWindow* window)
{
	return window->getVideoSize();
}

//---------------------------------------------------------------------------------------
VideoWindowPropertyVideoFPS::VideoWindowPropertyVideoFPS()
{
	mPropertyName = "VideoFPS";
	mDescription = "视频帧率";
	mReadOnly = true;
}
void VideoWindowPropertyVideoFPS::setRealValue(const float& value, VideoWindow* window)
{
	// 不实现设置帧率
}
float VideoWindowPropertyVideoFPS::getRealValue(VideoWindow* window)
{
	return window->getVideoFPS();
}