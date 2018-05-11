#include "txAudio.h"
#include "Utility.h"
#include "txAudioManager.h"
#include "txEngineRoot.h"

signed char F_CALLBACKAPI PlayOverEvent(FSOUND_STREAM *stream, void* buff, int len, void* userdata)
{
	if (userdata != NULL)
	{
		txAudio* player = (txAudio*)userdata;
		// 不是循环播放的音效则停止音效
		if (!player->isLoop())
		{
			player->stop();
		}
		// 循环播放的音效则继续从头开始播放
		else
		{
			player->play(player->getChannel());
		}
	}
	return 0;
}

txAudio::txAudio()
:
m_bLoop(false),
m_bIsCreate(false),
mState(PS_STOP),
mDefaultVolume(100.0f),
mLength(0.0f)
{
	mStream = NULL;
	mChannel = -1;
}

txAudio::~txAudio()
{
	stop();
	if (mStream != NULL)
	{
		FSOUND_Stream_Close(mStream);
		mStream = NULL;
	}
}

bool txAudio::initialize(const std::string& fileName, const std::string& name)
{
	m_strName = name;
	mStream = FSOUND_Stream_Open(fileName.c_str(), 0, 0, 0);
	if (mStream == NULL)
	{
		m_bIsCreate = false;
		ENGINE_INFO("error : can not open audio file : %s", fileName.c_str());
		return false;
	}
	m_bIsCreate = true;

	FSOUND_Stream_SetEndCallback(mStream, PlayOverEvent, this);

	// 设置默认音量
	setVolume(mDefaultVolume);
	mLength = FSOUND_Stream_GetLengthMs(mStream) / 1000.0f;
	return true;
}
void txAudio::play(const int& channel)
{
	// 非暂停状态下,直接播放
	if (mState != PS_PAUSE)
	{
		if (mStream != NULL)
		{
			mChannel = channel;
			FSOUND_Stream_Play(mChannel, mStream);
			setVolume(mVolume);
		}
	}
	// 暂停状态则恢复继续播放
	else
	{
		pause(false);
	}
	mState = PS_PLAY;
}

void txAudio::setVolume(const float& volume)
{
	if (mState == PS_PLAY)
	{
		mVolume = volume;
		txMath::clamp(mVolume, 0.0f, 255.0f);
		if (mStream != NULL)
		{
			FSOUND_SetVolume(mChannel, (int)mVolume);
		}
	}
}

void txAudio::stop()
{
	if (mState != PS_STOP && mChannel != -1)
	{
		FSOUND_Stream_Stop(mStream);
		mState = PS_STOP;
		// 通知音效管理器音效停止了
		mAudioManager->notifyAudioStop(this);
	}
}

void txAudio::pause(const bool& paused)
{
	if (mState == PS_STOP)
	{
		return;
	}
	if (paused && mState == PS_PLAY)
	{
		mState = PS_PAUSE;
	}
	else
	{
		mState = PS_PLAY;
	}
	FSOUND_SetPaused(mChannel, mState == PS_PAUSE);
}

void txAudio::setCurTime(const float& curTime)
{
	FSOUND_Stream_SetTime(mStream, (int)curTime);
}