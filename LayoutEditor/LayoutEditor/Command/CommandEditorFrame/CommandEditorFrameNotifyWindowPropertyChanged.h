#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_PROPERTY_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_WINDOW_PROPERTY_CHANGED_H_

#include "EditorCommand.h"

// 通知EditorFrame正在编辑的Layout有改变
class CommandEditorFrameNotifyWindowPropertyChanged : public EditorCommand
{
public:
	virtual void reset()
	{
		mWindowName = EMPTY_STRING;
		mPropertyName = EMPTY_STRING;
		mOldPropertyValue = EMPTY_STRING;
		mPropertyValue = EMPTY_STRING;
		mChanger = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mWindowName;
	std::string mPropertyName;		// 属性名
	std::string mOldPropertyValue;	// 修改前的属性值
	std::string mPropertyValue;		// 修改后的属性值
	void* mChanger;
};

#endif