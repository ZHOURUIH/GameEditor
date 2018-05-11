#ifndef _TX_AUDIO_H_
#define _TX_AUDIO_H_

#include "txEngineDefine.h"
#include "txEngineBase.h"

#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

class txAudio : public txEngineBase
{
public:
	txAudio();
	virtual ~txAudio();

	const bool& isCreated()			{ return m_bIsCreate; }
	const bool& isLoop()			{ return m_bLoop; }
	const PLAY_STATE& getStatus()	{ return mState; }
	const std::string& getName()	{ return m_strName; }
	const float& getLength()		{ return mLength; }
	const int& getChannel()			{ return mChannel; }

	// fileName是media下的路径
	bool initialize(const std::string& fileName, const std::string& name);
	// 播放音效需要指定一个通道,不过只有在非暂停状态下才会重新设置通道
	void play(const int& channel);
	// 音量的取值范围是0到255
	void setVolume(const float& volume);
	void setLoop(const bool& loop){m_bLoop = loop;}
	void setChannel(const int& channel) { mChannel = channel; }
	void stop();
	void pause(const bool& paused = true);
	void setCurTime(const float& curTime);
protected:
	// fmod的SOUND支持wav,mp3,ogg,raw
	FSOUND_STREAM* mStream;
	int mChannel;
	float mLength;
	bool m_bLoop;
	bool m_bIsCreate;
	PLAY_STATE mState;
	std::string m_strName;
	float mDefaultVolume;
	float mVolume;
};

#endif