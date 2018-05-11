#ifndef _COMMAND_GAME_SCENE_PLAY_AUDIO_H_
#define _COMMAND_GAME_SCENE_PLAY_AUDIO_H_

#include "txCommand.h"

// ≤•∑≈”Œœ∑±≥æ∞“Ù¿÷
class CommandGameScenePlayAudio : public txCommand
{
public:
	virtual void reset()
	{
		mSoundFileName = EMPTY_STRING;
		mLoop = false;
		mVolume = 255.0f;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mSoundFileName;
	bool mLoop;
	float mVolume;
};

#endif