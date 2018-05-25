#ifndef _COMMAND_SCENE_SELECT_OBJECT_H_
#define _COMMAND_SCENE_SELECT_OBJECT_H_

#include "EditorCoreCommand.h"

class CommandSceneSelectObject : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mFocus = true;
		mObjectName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mFocus;
	std::string mObjectName;
};

#endif