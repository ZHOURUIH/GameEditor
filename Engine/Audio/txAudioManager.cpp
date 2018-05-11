#include "txAudioManager.h"
#include "txEngineLog.h"
#include "txEngineRoot.h"
#include "txAudio.h"
#include "Utility.h"

txAudioManager::txAudioManager() 
:
mCounter(0)
{
	;
}

txAudioManager::~txAudioManager()
{
	clear();
	FSOUND_Close();
}

void txAudioManager::init()
{
	FSOUND_Init(44100, MAX_CHANNEL_COUNT, 0);
	for (int i = 0; i < MAX_CHANNEL_COUNT; ++i)
	{
		mChannelPool[i] = false;
	}
}

bool txAudioManager::createAudioPlayer(const std::string &_filepath, const std::string& _name)
{
	if (NULL != find(_name))
	{
		return false;
	}
	txAudio* audioPlayer = TRACE_NEW(txAudio, audioPlayer);
	bool ret = audioPlayer->initialize(_filepath, _name);
	if (!ret)
	{
		return false;
	}

	mAudioPlayerMap.insert(_name, audioPlayer);
	return true;
}

bool txAudioManager::play(const std::string& name, const bool& loop, const float& volume)
{
	txAudio* audioPlayer = find(name);
	if (NULL == audioPlayer)
	{
		return false;
	}

	// 如果该音效正在播放,则直接返回
	if (audioPlayer->getStatus() == PS_PLAY)
	{
		return false;
	}

	// 找到一个可用的通道来播放音效
	int channel = -1;
	for (int i = 0; i < MAX_CHANNEL_COUNT; ++i)
	{
		if (!mChannelPool[i])
		{
			channel = i;
			break;
		}
	}
	if (channel == -1)
	{
		ENGINE_ERROR("error : can not assign a unused channel to sound! sound name : %s", name.c_str());
		return false;
	}
	mChannelPool[channel] = true;
	audioPlayer->play(channel);
	audioPlayer->setLoop(loop);
	audioPlayer->setVolume(volume);
	// 计数正在播放的声音
	++mCounter;
	return true;
}

void txAudioManager::setVolume(const std::string& name, const float& volume)
{
	txAudio* audio = find(name);
	if (audio != NULL)
	{
		audio->setVolume(volume);
	}
}

void txAudioManager::setCurrenTime(const std::string& name, const float& time)
{
	txAudio* audio = find(name);
	if (audio != NULL)
	{
		audio->setCurTime(time);
	}
}
void txAudioManager::setLoop(const std::string& name, const bool& loop)
{
	txAudio* audio = find(name);
	if (audio != NULL)
	{
		audio->setLoop(loop);
	}
}
void txAudioManager::pause(const std::string& name)
{
	txAudio* audio = find(name);
	if (audio != NULL)
	{
		audio->pause(true);
	}
}
void txAudioManager::resume(const std::string& name)
{
	txAudio* audio = find(name);
	if (audio != NULL)
	{
		audio->play(audio->getChannel());
	}
}
void txAudioManager::stop(const std::string& name)
{
	txAudio* audio = find(name);
	if (audio != NULL)
	{
		audio->stop();
	}
}
bool txAudioManager::created(const std::string& name)
{
	txAudio* audioPlayer = find(name);
	if (NULL == audioPlayer)
	{
		return false;
	}
	return audioPlayer->isCreated();
}

txAudio* txAudioManager::find(const std::string& name)
{
	auto itrFind = mAudioPlayerMap.find(name);
	if (itrFind != mAudioPlayerMap.end())
	{
		return itrFind->second;
	}
	return NULL;
}

void txAudioManager::stopAll()
{
	auto begin = mAudioPlayerMap.begin();
	auto itrEnd = mAudioPlayerMap.end();
	FOR_STL(mAudioPlayerMap, ; begin != itrEnd; ++begin)
	{
		begin->second->stop();
	}
	END_FOR_STL(mAudioPlayerMap);
}

void txAudioManager::remove(const std::string& _name)
{
	auto begin = mAudioPlayerMap.find(_name);
	if (begin != mAudioPlayerMap.end())
	{
		stop(_name);
		TRACE_DELETE(begin->second);
		mAudioPlayerMap.erase(begin);
	}
}

void txAudioManager::clear()
{
	stopAll();
	auto begin = mAudioPlayerMap.begin();
	auto itrEnd = mAudioPlayerMap.end();
	FOR_STL(mAudioPlayerMap, ; begin != itrEnd; ++begin)
	{
		TRACE_DELETE(begin->second)
	}
	END_FOR_STL(mAudioPlayerMap);
	mAudioPlayerMap.clear();
}

void txAudioManager::notifyAudioStop(txAudio* audio)
{
	--mCounter;
	// 停止播放音效后需要将该音效的通道进行回收
	int channel = audio->getChannel();
	mChannelPool[channel] = false;
	audio->setChannel(-1);
}