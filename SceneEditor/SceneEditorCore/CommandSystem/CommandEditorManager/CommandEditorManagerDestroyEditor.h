#ifndef _COMMAND_EDITOR_MANAGER_DESTROY_EDITOR_H_
#define _COMMAND_EDITOR_MANAGER_DESTROY_EDITOR_H_

#include "EditorCoreCommand.h"

class CommandEditorManagerDestroyEditor : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mEditor = NULL;
		mEditorName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	ECEditor* mEditor;			// 编辑体指针和名字任填一个就行,如果都填了,则使用编辑体指针
	std::string mEditorName;
};

#endif