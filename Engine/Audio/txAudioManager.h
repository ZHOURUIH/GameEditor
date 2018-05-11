#ifndef _TX_AUDIO_MANAGER_H_
#define _TX_AUDIO_MANAGER_H_

#include "txEngineDefine.h"
#include "fmod/fmod.h"
#include "fmod/fmod_errors.h"

class txAudio;
class txAudioManager
{
public:
	txAudioManager();
	virtual ~txAudioManager();
	virtual void update(float elaspedTime){}
	void init();
	bool createAudioPlayer(const std::string& filepath, const std::string& name);
	bool play(const std::string& name, const bool& loop = false, const float& volume = 255.0f);
	void setVolume(const std::string& name, const float& volume);
	void setCurrenTime(const std::string& name, const float& time);
	void setLoop(const std::string& name, const bool& loop);
	void pause(const std::string& name);
	void resume(const std::string& name);
	void stop(const std::string& name);
	void stopAll();
	bool created(const std::string& name);
	txAudio* find(const std::string& name);
	void remove(const std::string& name);
	void clear();

	// 通知管理器音效停止播放了
	void notifyAudioStop(txAudio* audio);
private:
	// first是通道下标,second是该通道是否使用
	bool mChannelPool[MAX_CHANNEL_COUNT];
	txMap<std::string, txAudio*> mAudioPlayerMap;
	int mCounter; // 正在播放的声音数量
};

#endif