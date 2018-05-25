#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_ADDED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_LAYOUT_ADDED_H_

#include "EditorCommand.h"

// 通知EditorFrame正在编辑的Layout有改变
class CommandEditorFrameNotifyLayoutAdded : public EditorCommand
{
public:
	virtual void reset(){}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mName;			// 被删除的窗口名字
};

#endif