#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_UNDO_ENABLE_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_UNDO_ENABLE_CHANGED_H_

#include "EditorCommand.h"

// 通知EditorFrame可撤销状态改变
class CommandEditorFrameNotifyUndoEnableChanged : public EditorCommand
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