#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_CUTED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_CUTED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyWindowCuted : public EditorCommand
{
public:
	virtual void reset()
	{
		mCutWindowName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mCutWindowName;
};

#endif