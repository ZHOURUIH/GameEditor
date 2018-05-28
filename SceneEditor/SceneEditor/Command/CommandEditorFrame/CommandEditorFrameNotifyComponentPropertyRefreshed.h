#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_COMPONENT_PROPERTY_REFRESHED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_COMPONENT_PROPERTY_REFRESHED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyComponentPropertyRefreshed : public EditorCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
};

#endif