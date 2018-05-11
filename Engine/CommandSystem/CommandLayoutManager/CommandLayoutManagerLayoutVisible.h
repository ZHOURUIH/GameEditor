#ifndef _COMMAND_LAYOUT_MANAGER_LAYOUT_VISIBLE_H_
#define _COMMAND_LAYOUT_MANAGER_LAYOUT_VISIBLE_H_

#include "txCommand.h"

class CommandLayoutManagerLayoutVisible : public txCommand
{
public:
	virtual void reset()
	{
		mLayoutID = -1;
		mLayoutName = EMPTY_STRING;
		mForce = false;
		mImmediately = false;
		mVisibility = true;
		mParam = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	int			mLayoutID;
	std::string mLayoutName;
	bool		mForce;
	bool		mImmediately;
	bool		mVisibility;
	std::string	mParam;
};

#endif