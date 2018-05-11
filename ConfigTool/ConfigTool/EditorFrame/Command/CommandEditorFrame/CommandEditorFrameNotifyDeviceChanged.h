#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_DEVICE_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_DEVICE_CHANGED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyDeviceChanged : public EditorCommand
{
public:
	COMMAND_EDITOR_CONSTRUCT(CommandEditorFrameNotifyDeviceChanged)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
};

#endif