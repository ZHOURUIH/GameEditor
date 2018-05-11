#ifndef _COMMAND_GAME_SCENE_CHANGE_AUDIO_VOLUME_H_
#define _COMMAND_GAME_SCENE_CHANGE_AUDIO_VOLUME_H_

#include "txCommand.h"

class CommandGameSceneChangeAudioVolume : public txCommand
{
public:
	virtual void reset()
	{
		mVolume = 255.0f;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	float mVolume;
};

#endif