#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_ADDED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_ADDED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyWindowAdded : public EditorCommand
{
public:
	virtual void reset()
	{
		mParentName = EMPTY_STRING;
		mName = EMPTY_STRING;
		mWindowPos = 0;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mParentName;	// 添加的父窗口的名字
	std::string mName;			// 窗口名字
	int mWindowPos;
};

#endif