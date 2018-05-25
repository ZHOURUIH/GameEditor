#ifndef _COMMAND_SCENE_NEW_EDITOR_H_
#define _COMMAND_SCENE_NEW_EDITOR_H_

#include "EditorCoreCommand.h"

class CommandSceneNewEditor : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mEditorName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mEditorName;
};

#endif