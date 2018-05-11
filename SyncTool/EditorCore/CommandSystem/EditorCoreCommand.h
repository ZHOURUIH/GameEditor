#ifndef _EDITOR_CORE_COMMAND_H_
#define _EDITOR_CORE_COMMAND_H_

#include "Command.h"

#include "EditorCoreBase.h"

#define COMMAND_EDITOR_CORE_CONSTRUCT(className) COMMAND_CONSTRUCT(className, EditorCoreCommand)

class EditorCoreCommand : public EditorCoreBase, public Command
{
public:
	EditorCoreCommand(const char* file, int line, bool showInfo = true)
		:
		Command(file, line, showInfo)
	{}
};

#endif