#ifndef _COMMAND_WINDOW_ACTIVE_H_
#define _COMMAND_WINDOW_ACTIVE_H_

#include "txCommand.h"

class CommandWindowActive : public txCommand
{
public:
	virtual void reset()
	{
		mActive = true;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mActive;
};

#endif