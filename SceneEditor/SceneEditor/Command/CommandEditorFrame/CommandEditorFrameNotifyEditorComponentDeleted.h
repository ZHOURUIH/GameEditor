#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_EDITOR_COMPONENT_DELETED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_EDITOR_COMPONENT_DELETED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyEditorComponentDeleted : public EditorCommand
{
public:
	virtual void reset()
	{
		mEditorName = EMPTY_STRING;
		mComponentName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mEditorName;
	std::string mComponentName;
};

#endif