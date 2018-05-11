#ifndef _COMMAND_GAME_SCENE_PREPARE_CHANGE_PROCEDURE_H_
#define _COMMAND_GAME_SCENE_PREPARE_CHANGE_PROCEDURE_H_

#include "txCommand.h"

class CommandGameScenePrepareChangeProcedure : public txCommand
{
public:
	virtual void reset()
	{
		mProcedure = -1;
		mIntent = EMPTY_STRING;
		mPrepareTime = 0.0f;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	int mProcedure;
	std::string mIntent;
	float mPrepareTime;
};

#endif