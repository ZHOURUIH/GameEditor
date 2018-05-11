#ifndef _COMMAND_WINDOW_PLAY_AUDIO_H_
#define _COMMAND_WINDOW_PLAY_AUDIO_H_

#include "txCommand.h"

class CommandWindowPlayAudio : public txCommand
{
public:
	virtual void reset()
	{
		mSoundName = EMPTY_STRING;
		mLoop = false;
		mVolume = 255.0f;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mSoundName;
	bool mLoop;
	float mVolume;// 范围0-255
};

#endif