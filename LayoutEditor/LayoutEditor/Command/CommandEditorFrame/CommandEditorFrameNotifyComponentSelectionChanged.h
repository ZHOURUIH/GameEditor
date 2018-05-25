#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_COMPONENT_SELECTION_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_COMPONENT_SELECTION_CHANGED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyComponentSelectionChanged : public EditorCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif