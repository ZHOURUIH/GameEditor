#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_RENAME_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_RENAME_H_

#include "EditorCommand.h"

// 通知EditorFrame正在编辑的Layout有改变
class CommandEditorFrameNotifyWindowRename : public EditorCommand
{
public:
	virtual void reset()
	{
		mOldName = EMPTY_STRING;
		mNewName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mOldName;	// 被删除的父窗口的名字
	std::string mNewName;	// 被删除的窗口名字
};

#endif