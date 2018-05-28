#ifndef _COMMAND_EDITOR_FRAME_NOTIFY_COMPONENT_PROPERTY_CHANGED_H_
#define _COMMAND_EDITOR_FRAME_NOTIFY_COMPONENT_PROPERTY_CHANGED_H_

#include "EditorCommand.h"

class CommandEditorFrameNotifyComponentPropertyChanged : public EditorCommand
{
public:
	virtual void reset()
	{
		mEditorName = EMPTY_STRING;
		mComponentName = EMPTY_STRING;
		mPropertyName = EMPTY_STRING;
		mOldPropertyValue = EMPTY_STRING;
		mPropertyValue = EMPTY_STRING;
		mChanger = NULL;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mEditorName;
	std::string mComponentName;
	std::string mPropertyName;		// 属性名
	std::string mOldPropertyValue;	// 修改前的属性值
	std::string mPropertyValue;		// 修改后的属性值
	void* mChanger;
};

#endif