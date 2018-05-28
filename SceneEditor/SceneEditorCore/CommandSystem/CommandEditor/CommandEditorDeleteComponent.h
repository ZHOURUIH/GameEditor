#ifndef _COMMAND_EDITOR_DELETE_COMPONENT_H_
#define _COMMAND_EDITOR_DELETE_COMPONENT_H_

#include "EditorCoreCommand.h"

class CommandEditorDeleteComponent : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mComponentName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mComponentName;
};

#endif