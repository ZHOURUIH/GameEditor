#ifndef _COMMAND_EDITOR_ADD_COMPONENT_H_
#define _COMMAND_EDITOR_ADD_COMPONENT_H_

#include "EditorCoreCommand.h"

class txComponent;
class CommandEditorAddComponent : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mComponent = NULL;
		mType = EMPTY_STRING;
		mComponentName = EMPTY_STRING;
		mParentComponentName = EMPTY_STRING;
		mComponentPos = -1;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	txComponent* mComponent;	// 源组件,如果该变量不为空,则添加的组件类型取该类型,并且添加组件后将该组件的所有属性拷贝到新组件中
	std::string mType;
	std::string mComponentName;
	std::string mParentComponentName;
	int mComponentPos;
};

#endif