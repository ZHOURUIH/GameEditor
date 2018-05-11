#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_PANEL_SHOW_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_PANEL_SHOW_H_

#include "EditorCommand.h"

// 通知EditorFrame编辑器中有窗口显示状态改变
class CommandEditorFrameNotifyPanelShow : public EditorCommand
{
public:
	COMMAND_EDITOR_CONSTRUCT(CommandEditorFrameNotifyPanelShow)
	,mShow(true)
	{}
	COMMAND_DECLARE_FUNCTION;
public:
	std::string mPanelName;
	bool mShow;
};

#endif