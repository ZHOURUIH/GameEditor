#ifndef _COMMAND_EDITOR_MANAGER_CREATE_EDITOR_H_
#define _COMMAND_EDITOR_MANAGER_CREATE_EDITOR_H_

#include "EditorCoreCommand.h"

class ECEditor;
class CommandEditorManagerCreateEditor : public EditorCoreCommand
{
public:
	virtual void reset()
	{
		mName = EMPTY_STRING;
	}
	virtual void execute();
	virtual std::string showDebugInfo();
public:
	std::string mName;
};
	
#endif