#ifndef _COMMAND_EDITOR_RENAME_COMPONENT_H_
#define _COMMAND_EDITOR_RENAME_COMPONENT_H_

#include "EditorCoreCommand.h"

class CommandEditorRenameComponent : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mComponentName = EMPTY_STRING;
		mNewName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mComponentName;
	std::string mNewName;
};

#endif