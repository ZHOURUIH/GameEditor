#ifndef _COMMAND_WINDOW_STOP_AUDIO_H_
#define _COMMAND_WINDOW_STOP_AUDIO_H_

#include "txCommand.h"

class CommandWindowStopAudio : public txCommand
{
public:
	virtual void reset()
	{
		;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif