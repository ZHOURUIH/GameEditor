#ifndef _EDITOR_CORE_COMMAND_H_
#define _EDITOR_CORE_COMMAND_H_

#include "txCommand.h"

#include "EditorCoreBase.h"

class EditorCoreCommand : public EditorCoreBase, public txCommand
{
public:
	EditorCoreCommand(const char* file, int line, bool showInfo = true)
		:
		txCommand(file, line, showInfo)
	{}
};

#endif