#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_EDITOR_ADDED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_EDITOR_ADDED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyEditorAdded : public EditorCommand
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