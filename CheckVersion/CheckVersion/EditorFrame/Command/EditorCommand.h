#ifndef _EDITOR_COMMAND_H_
#define _EDITOR_COMMAND_H_

#include "ToolCoreCommand.h"
#include "EditorBase.h"

#define COMMAND_EDITOR_CONSTRUCT(className) COMMAND_CONSTRUCT(className, EditorCommand)

class EditorCommand : public ToolCoreCommand, public EditorBase
{
public:
	EditorCommand(const char* file, int line, bool showInfo = true)
		:
		ToolCoreCommand(file, line, showInfo)
	{}
};

#endif