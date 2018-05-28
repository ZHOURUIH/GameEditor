#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_DELETE_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_DELETE_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyWindowDelete : public EditorCommand
{
public:
	virtual void reset()
	{
		mParentName = EMPTY_STRING;
		mName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mParentName;	// 被删除的父窗口的名字
	std::string mName;			// 被删除的窗口名字
};

#endif