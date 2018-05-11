#ifndef _COMMAND_GAME_SCENE_CHANGE_PROCEDURE_H_
#define _COMMAND_GAME_SCENE_CHANGE_PROCEDURE_H_

#include "txCommand.h"

class CommandGameSceneChangeProcedure : public txCommand
{
public:
	virtual void reset()
	{
		mProcedure = -1;
		mIntent = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	int mProcedure;
	std::string mIntent;
};

#endif