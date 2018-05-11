#ifndef _COMMAND_LAYOUT_MANAGER_UNLOAD_LAYOUT_H_
#define _COMMAND_LAYOUT_MANAGER_UNLOAD_LAYOUT_H_

#include "txCommand.h"

class CommandLayoutManagerUnloadLayout : public txCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
		mLayoutID = -1;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mName;
	int mLayoutID;
};

#endif