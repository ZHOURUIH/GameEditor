#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_CLOSED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_CLOSED_H_

#include "EditorCommand.h"

// 通知EditorFrame有一个Layout被关闭了
class CommandEditorFrameNotifyLayoutClosed : public EditorCommand
{
public:
	virtual void reset()
	{
		mLayoutName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mLayoutName;
};

#endif