#ifndef _COMMAND_GAME_SCENE_BACK_TO_LAST_PROCEDURE_H_
#define _COMMAND_GAME_SCENE_BACK_TO_LAST_PROCEDURE_H_

#include "txCommand.h"

class CommandGameSceneBackToLastProcedure : public txCommand
{
public:
	virtual void reset()
	{
		mIntent = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mIntent;
};

#endif