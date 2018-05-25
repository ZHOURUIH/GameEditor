#ifndef _COMMAND_SCENE_SHOW_H_
#define _COMMAND_SCENE_SHOW_H_

#include "EditorCoreCommand.h"

class CommandSceneShow : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mShow = true;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mShow;
};

#endif