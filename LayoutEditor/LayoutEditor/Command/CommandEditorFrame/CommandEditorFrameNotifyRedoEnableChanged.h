#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_REDO_ENABLE_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_REDO_ENABLE_CHANGED_H_

#include "EditorCommand.h"

// 通知EditorFrame可重做状态改变
class CommandEditorFrameNotifyRedoEnableChanged : public EditorCommand
{
public:
	virtual void reset()
	{
		mEnabled = true;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mEnabled;
};

#endif