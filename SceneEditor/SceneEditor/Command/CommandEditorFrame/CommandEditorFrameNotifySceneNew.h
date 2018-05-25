#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_SCENE_NEW_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_SCENE_NEW_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifySceneNew : public EditorCommand
{
public:
	virtual void reset()
	{
		mSceneName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mSceneName;
};

#endif