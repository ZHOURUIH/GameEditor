#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_EDITOR_COMPONENT_ADDED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_EDITOR_COMPONENT_ADDED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyEditorComponentAdded : public EditorCommand
{
public:
	virtual void reset()
	{
		mEditorName = EMPTY_STRING;
		mComponentName = EMPTY_STRING;
		mComponentPos = -1;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mEditorName;
	std::string mComponentName;
	int mComponentPos;
};

#endif