#ifndef _TX_COMPONENT_AUDIO_H_
#define _TX_COMPONENT_AUDIO_H_

#include "txAudioManager.h"
#include "txComponent.h"

class txComponentAudio : public txComponent
{
public:
	txComponentAudio(const std::string& type, const std::string& name)
		:
		txComponent(type, name)
	{}
	virtual ~txComponentAudio(){ destroy(); }
	virtual void setBaseType(){ mBaseType = TOSTRING(txComponentAudio); }
	virtual bool isType(const std::string& type){ return type == TOSTRING(txComponentAudio); }
	void destroy(){}
	void setLoop(const bool& loop = false)
	{
		mAudioManager->setLoop(mCurAudio, loop);
	}
	void setVolume(const float& vol)
	{
		mAudioManager->setVolume(mCurAudio, vol);
	}
	virtual void play(const std::string& name, const bool& isLoop = false, const float& volume = 255.0f)
	{
		mCurAudio = name;
		mAudioManager->play(mCurAudio, isLoop, volume);
	}
	virtual void stop()
	{
		mAudioManager->stop(mCurAudio);
	}
protected:
	std::string mCurAudio;
};

#endif