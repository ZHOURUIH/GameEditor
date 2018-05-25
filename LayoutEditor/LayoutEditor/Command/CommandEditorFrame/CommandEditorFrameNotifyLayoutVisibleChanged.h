#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_VISIBLE_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_VISIBLE_CHANGED_H_

#include "EditorCommand.h"

// 通知EditorFrame正在编辑的Layout的显示状态有改变
class CommandEditorFrameNotifyLayoutVisibleChanged : public EditorCommand
{
public:
	virtual void reset()
	{
		mVisible = true;
		mLayoutName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	bool mVisible;
	std::string mLayoutName;
};

#endif