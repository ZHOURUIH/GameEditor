#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_EDITOR_SELECTION_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_EDITOR_SELECTION_CHANGED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyEditorSelectionChanged : public EditorCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif