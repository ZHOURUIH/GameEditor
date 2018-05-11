#ifndef _EDITOR_COMMAND_H_
#define _EDITOR_COMMAND_H_

#include "EditorCoreCommand.h"
#include "EditorBase.h"

#define COMMAND_EDITOR_CONSTRUCT(className) COMMAND_CONSTRUCT(className, EditorCommand)

class EditorCommand : public EditorCoreCommand, public EditorBase
{
public:
	EditorCommand(const char* file, int line, bool showInfo = true)
		:
		EditorCoreCommand(file, line, showInfo)
	{}
};

#endif