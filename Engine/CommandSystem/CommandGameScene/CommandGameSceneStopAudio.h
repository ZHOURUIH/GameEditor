#ifndef _COMMAND_GAME_SCENE_STOP_AUDIO_H_
#define _COMMAND_GAME_SCENE_STOP_AUDIO_H_

#include "txCommand.h"

class CommandGameSceneStopAudio : public txCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif